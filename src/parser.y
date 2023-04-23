%skeleton "lalr1.cc" // -*- C++ -*-
%require "3.8.1"

%defines
%define api.namespace { Pseudo }
%define api.parser.class { Parser }

%code requires {
    namespace Pseudo {
        class Interpreter;
        class Scanner;
    };

    #include "node.h"
}

%parse-param { Scanner& scanner  }
%parse-param { Interpreter& driver  }

%code {
    #include <iostream>
    #include <cstdlib>
    #include <fstream>

    #include "interpreter.h"
    #include "node.h"

    #undef yylex
    #define yylex scanner.yylex
}

/*define api.value.type variant*/
%define parse.assert
%define parse.error detailed

%union {
    Pseudo::Node* node;
    Pseudo::Block* block;
    Pseudo::Statement* stmt;
    Pseudo::Expression* expr;
    Pseudo::ExpressionList* exprlist;
    Pseudo::VectorAccess* vecaccess;
    Pseudo::Identifier* ident;
    std::string* string;
    long long integer;
    double rational;
    bool boolean;
    int token;
}

%token T_PROGRAM
%token T_VOID
%token T_FUNCTION
%token T_END
%token T_IF
%token T_ELSE
%token T_THEN
%token T_IF_END
%token T_ITERATE
%token T_WHILE
%token T_FROM
%token T_TO
%token T_RETURN 
%token T_BOOLEAN
%token T_INTEGER
%token T_RATIONAL
%token T_ARRAY 
%token T_PRINT
%token T_STRING
%token T_NEWLINE
%token <token> T_ASSIGN T_SWAP
%token <token> T_EQ T_NOTEQ T_LESS T_LESSEQ T_GREATER T_GREATEREQ
%token <token> T_ADD T_MINUS T_DIV T_MUL T_MODULUS
%token <token> T_AND T_OR
%token <token> T_LPAREN T_RPAREN T_LSBRACKET T_RSBRACKET T_COMMA T_DOT T_NOT T_COLON T_QUESTIONMARK
%token <string> T_IDENTIFIER T_STRING_VAL T_TRUE T_FALSE
%token <integer> T_INTEGER_VAL
%token <rational> T_RATIONAL_VAL
%token <token> T_INDENT T_OUTDENT
%token T_ENDOFFILE
%token T_CREATEVECTOR
%token T_CREATEMATRIX

%type <block> program stmts block
%type <stmt> stmt var_decl assign_stmt conditional iterate_stmt

// exprs
%type <expr> expr numeric logical func_call matrix vector
%type <vecaccess> vector_access matrix_access
%type <ident> type_specifier ident

%type <exprlist> call_args

%left T_OR
%left T_AND
%left T_EQ T_NOTEQ
%left T_LESS T_GREATER T_LESSEQ T_GREATEREQ
%left T_ADD T_MINUS
%left T_MUL T_DIV T_MODULUS
%precedence T_NOT

%start program

%locations

%%

program:
    %empty
    {
        driver.set_ast_root(new Pseudo::Block());
    }
|
    stmts
    {
        driver.set_ast_root($1);
        std::cout << "stmts <<EOF>>" << std::endl;
    }
;

stmts:
    stmt
    {
        $$ = new Pseudo::Block();
        $$->statements.push_back($<stmt>1);
        std::cout << "stmts -> stmt" << std::endl;
    }
|   
    stmts stmt
    {
        $1->statements.push_back($<stmt>2);
        std::cout << "stmts -> stmts stmt" << std::endl;
    }
;

ident : T_IDENTIFIER { $$ = new Pseudo::Identifier(*$1); }
      ;

stmt:
    var_decl close
    {
        std::cout << "stmts -> var_decl close" << std::endl;
    }
|
    func_decl
    {
        std::cout << "stmts -> func_decl close" << std::endl;
    }
|
    assign_stmt close
    {
        std::cout << "stmts -> assign_stmt close" << std::endl;
    }
|
    expr close
    {
        std::cout << "stmts -> expr close" << std::endl;
        $$ = new Pseudo::ExpressionStatement($1);
    }
|
    conditional close
    {
        std::cout << "stmts -> conditional close" << std::endl;
    }
|
    iterate_stmt close
    {
        std::cout << "stmts -> iterate_stmt close" << std::endl;
    }
|
    return close
    {
        std::cout << "stmts -> return close" << std::endl;
    }
;

func_decl:
    T_FUNCTION ident T_LPAREN func_decl_args T_RPAREN close block T_FUNCTION T_END
    {
        std::cout << "func_decl -> T_FUNCTION ident T_LPAREN func_decl_args T_RPAREN block T_FUNCTION T_END" << std::endl;
    }
;

func_decl_args :
    %empty 
    { 
        std::cout << "function_decl_args -> empty" << std::endl;
    }
|
    var_decl
    {
        std::cout << "func_decl_args -> var_decl" << std::endl;
    }
|
    func_decl_args T_COMMA var_decl
    {
        std::cout << "funcition_decl_args -> func_decl_args T_COMMA var_decl" << std::endl;
    }
;

var_decl :
    ident T_COLON ident T_ASSIGN expr  //type
    {
        $$ = new Pseudo::VariableDeclarationStatement($1, $3, $<expr>4);
        std::cout << "var_decl -> ident T_COLON ident T_ASSIGN expression" << std::endl;
    }
|   ident T_COLON ident  //type
    {
        $$ = new Pseudo::VariableDeclarationStatement($1, $3);
        std::cout << "var_decl -> ident T_COLON ident"  << std::endl;
    }
;
	
assign_stmt:
    ident T_ASSIGN expr
    {
        $$ = new Pseudo::AssignmentStatement($<ident>1, $<expr>3);
        std::cout << "assign_statement -> ident T_ASSIGN expr" << std::endl;
    }
|   
    vector_access T_ASSIGN expr
    {
        $$ = new Pseudo::ModifyVector($1, $3);
    }
|
    matrix_access T_ASSIGN expr
    {
        $$ = new Pseudo::ModifyVector($1, $3);
    }
;

conditional:
    T_IF expr T_THEN close block T_IF_END T_END
    {
        $$ = new ConditionalStatement($2, $5);
        std::cout << "conditional -> T_IF expr T_THEN block T_IF_END T_END" << std::endl;
    }
|
    T_IF expr T_THEN close block T_ELSE close block T_IF_END T_END
    {
        $$ = new ConditionalStatement($2, $5, $8);
        std::cout << "conditional -> T_IF expr T_THEN block T_ELSE block T_IF_END T_END" << std::endl;
    }
;

iterate_stmt:
    T_ITERATE T_WHILE expr close block T_ITERATE T_END
    {
        std::cout << "iterate_stmt -> T_ITERATE T_WHILE expr close block T_ITERATE T_END" << std::endl; 
    }
|
    T_ITERATE close block T_WHILE expr
    {
        std::cout << "iterate_stmt -> T_ITERATE close block T_WHILE expr T_ITERATE T_END" << std::endl;
    }
|
    T_ITERATE ident T_ASSIGN expr T_MINUS T_FROM expr T_MINUS T_TO close block T_ITERATE T_END 
    {
        std::cout << "iterate_stmt -> T_ITERATE ident T_ASSIGN expr T_FROM expr T_TO close block T_ITERATE T_END" << std::endl;
    }
;

return:
    T_RETURN expr
    {
        std::cout << "return -> T_RETURN expr" << std::endl;
    }
|
    T_RETURN
    {
        std::cout << "return -> T_RETURN" << std::endl;
    }
;


block:
    T_INDENT stmts T_OUTDENT
    {
        $$ = $2;
    }
|   T_INDENT T_OUTDENT
    {
        $$ = new Pseudo::Block();
    }

close:
    T_COMMA
    {
        std::cout << "close -> T_COMMA" << std::endl;
    }
|
    T_NEWLINE
    {
        std::cout << "close -> T_NEWLINE" << std::endl;
    }
|
    close T_NEWLINE
    {
        std::cout << "close -> close T_NEWLINE" << std::endl;
    }
;

expr:
    func_call 
    {
        std::cout << "expr -> func_call" << std::endl;
    }
|   
    ident
    {
        std::cout << "expr -> ident" << std::endl;
    }
|   
    numeric 
    {
        std::cout << "expr -> numeric" << std::endl;
    }
|   
    logical
    {
        std::cout << "expr -> logical" << std::endl;
    }
|   
    vector
    {
        std::cout << "expr -> vector" << std::endl;
    }
|
    matrix
    {
        std::cout << "expr -> matrix" << std::endl;
    }
|
    vector_access
    {
        std::cout << "expr -> vector_access" << std::endl;
    }
|
    matrix_access
    {
        std::cout << "expr -> matrix_access" << std::endl;
    }
|
    expr T_ADD expr
    {
        $$ = new BinaryOperator($1, $2, $3);
        std::cout << "expr -> expr T_ADD expr" << std::endl;
    }
|
    expr T_MINUS expr
    {
        $$ = new BinaryOperator($1, $2, $3);
        std::cout << "expr -> expr T_MINUS expr" << std::endl;
    }
|
    expr T_MUL expr
    {
        $$ = new BinaryOperator($1, $2, $3);
        std::cout << "expr -> expr T_MUL expr" << std::endl;
    }
|
    expr T_DIV expr
    {
        $$ = new BinaryOperator($1, $2, $3);
        std::cout << "expr -> expr T_DIV expr" << std::endl;
    }
|
    expr T_MODULUS expr
    {
        $$ = new BinaryOperator($1, $2, $3);
        std::cout << "expr -> expr T_MODULUS expr" << std::endl;
    }
|
    expr T_LESS expr
    {
        $$ = new BinaryOperator($1, $2, $3);
        std::cout << "expr -> expr T_LESS expr" << std::endl;
    }
|
    expr T_GREATER expr
    {
        $$ = new BinaryOperator($1, $2, $3);
        std::cout << "expr -> expr T_GREATER expr" << std::endl;
    }
|
    expr T_LESSEQ expr
    {
        $$ = new BinaryOperator($1, $2, $3);
        std::cout << "expr -> expr T_LESSEQ expr" << std::endl;
    }
|
    expr T_GREATEREQ expr
    {
        $$ = new BinaryOperator($1, $2, $3);
        std::cout << "expr -> expr T_GREATEREQ expr" << std::endl;
    }
|
    expr T_AND expr
    {
        $$ = new BinaryOperator($1, $2, $3);
        std::cout << "expr -> expr T_AND expr" << std::endl;
    }
|
    expr T_OR expr
    {
        $$ = new BinaryOperator($1, $2, $3);
        std::cout << "expr -> expr T_OR expr" << std::endl;
    }
|
    expr T_EQ expr
    {
        $$ = new BinaryOperator($1, $2, $3);
        std::cout << "expr -> expr T_EQ expr" << std::endl;
    }
|
    expr T_NOTEQ expr
    {
        $$ = new BinaryOperator($1, $2, $3);
        std::cout << "expr -> expr T_NOTEQ expr" << std::endl;
    }
|
    T_NOT expr
    {
        $$ = new UnaryOperator($1, $2);
        std::cout << "expr -> T_NOT expr" << std::endl;
    }
|   T_LPAREN expr T_RPAREN
    {
        $$ = $2;
    }
;

func_call:
    ident T_LPAREN call_args T_RPAREN
    {
        $$ = new Pseudo::FunctionCall($1, $3);
        std::cout << "func_call -> ident T_LPAREN call_args T_RPAREN" << std::endl;
    }
;

call_args:
    %empty
    { 
        $$ = new Pseudo::ExpressionList();
        std::cout << "call_args -> epsilon" << std::endl;
    }
    | expr 
    {
        $$ = new Pseudo::ExpressionList();
        $$->push_back($1);
        std::cout << "call_args -> expr" << std::endl;
    }
    | call_args T_COMMA expr 
    {
        $$->push_back($3);
        std::cout << "call_args -> call_args T_COMMA expr" << std::endl;
    } 
;

numeric:
    T_INTEGER_VAL
    {
        $$ = new Pseudo::Integer($1);
        std::cout << "numeric -> T_INTEGER_VAL" << std::endl;
    }
|   T_RATIONAL_VAL
    {
        $$ = new Pseudo::Rational($1);
        std::cout << "numeric -> T_RATIONAL_VAL" << std::endl;
    }
;

logical:
    T_TRUE
    {
        $$ = new Pseudo::Boolean(true);
        std::cout << "logical -> T_TRUE" << std::endl;
    }
|   T_FALSE
    {
        $$ = new Pseudo::Boolean(false);
        std::cout << "logical -> T_FALSE" << std::endl;
    }
;

vector:
    // 'Letrehoz(egesz)[6]' 
    T_CREATEVECTOR T_LPAREN type_specifier T_RPAREN T_LSBRACKET T_INTEGER_VAL T_RSBRACKET
    {
        // make sure type_spec is not void
        auto vec = new Pseudo::ExpressionList();
        $$ = new Pseudo::Vector($3, new Pseudo::ExpressionList(), $6);
        std::cout << "vector -> T_CREATEVECTOR T_LPAREN type_specifier T_RPAREN T_LSBRACKET expr T_RSBACKET" << std::endl;
    }
;

vector_access:
    ident T_LSBRACKET expr T_RSBRACKET
    {
        $$ = new VectorAccess($1, $3);
        std::cout << "vector_access -> ident T_LSBRACKET expr T_RSBRACKET" << std::endl;
    }
;

matrix:
    // 'TablaLetrehoz(egesz)[6, 5]'
    T_CREATEMATRIX T_LPAREN type_specifier T_RPAREN T_LSBRACKET T_INTEGER_VAL T_COMMA T_INTEGER_VAL T_RSBRACKET
    {
        auto exprs = new Pseudo::ExpressionList();
        for (int i = 0; i < $6; ++i)
        {
            exprs->push_back(new Pseudo::Vector($3, new Pseudo::ExpressionList(), $8));
        }

        $$ = new Pseudo::Vector($3, exprs, $6);

        // make sure exprs only int!
        std::cout << "matrix -> T_CREATEMATRIX T_LPAREN type_specifier T_RPAREN T_LSBRACKET expr T_COMMA expr T_RSBACKET" << std::endl;
        //std::cout << "type:" << $3 << "size: " << $6 << std::endl;
        std::cout << "type:" << "size: " << $6 << std::endl;
    }
;

matrix_access:
    ident T_LSBRACKET expr T_COMMA expr T_RSBRACKET
    {
        auto exprs = new ExpressionList();
        exprs->push_back($3);
        exprs->push_back($5);
        $$ = new Pseudo::VectorAccess($1, exprs);
        std::cout << "matrix_access -> ident T_LSBRACKET expr T_COMMA expr T_RSBRACKET" << std::endl;
    }
;

type_specifier:
    T_BOOLEAN
    {
        $$ = new Pseudo::Identifier("boolean");
        std::cout << "type_specifier -> T_BOOLEAN" << std::endl;
    }
|   T_INTEGER
    {
        $$ = new Pseudo::Identifier("integer");
        std::cout << "type_specifier -> T_INTEGER" << std::endl;
    }
|   T_RATIONAL
    {
        $$ = new Pseudo::Identifier("rational");
        std::cout << "type_specifier -> T_RATIONAL" << std::endl;
    }
|   T_STRING
    {
        $$ = new Pseudo::Identifier("string");
        std::cout << "type_specifier -> T_STRING" << std::endl;
    }
|   T_VOID
    {
        $$ = new Pseudo::Identifier("void");
        std::cout << "type_specifier -> T_VOID" << std::endl;
    }
;

%%

void Pseudo::Parser::error(const location_type& loc, const std::string& err_message)
{
	std::cout << loc.begin.line << loc.begin.column << std::endl;
	std::cout << loc.end.line << loc.end.column << std::endl;
    std::cerr << "Error: " << err_message << " at " << loc << std::endl;
}