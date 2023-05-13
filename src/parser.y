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
    #include "nodes/binaryoperator.h"
    #include "nodes/unaryoperator.h"
    #include "nodes/variabledeclaration.h"
    #include "nodes/compareoperator.h"
    #include "nodes/functiondeclaration.h"
    #include "nodes/assignment.h"
    #include "nodes/string.h"
    #include "nodes/vector.h"
    #include "nodes/forloop.h"
}

%parse-param { Scanner& scanner  }
%parse-param { Interpreter& driver  }

%locations

%code {
    #include <iostream>
    #include <cstdlib>
    #include <fstream>

    #include "location.hh"
    #include "interpreter.h"
    #include "node.h"
    #include "nodes/binaryoperator.h"
    #include "nodes/unaryoperator.h"
    #include "nodes/variabledeclaration.h"

    #undef yylex
    #define yylex scanner.yylex

    #define PSEUDO_PRINT_GRAMMAR 0

    void printRule(const std::string& rule, bool forcePrint = false)
    {
        if (PSEUDO_PRINT_GRAMMAR || forcePrint)
        {
            std::cout << rule << std::endl;
        }
    }
}

/*define api.value.type variant*/
%define parse.assert
%define parse.error detailed

%union {
    Pseudo::Node* node;
    Pseudo::Block* block;
    Pseudo::Statement* stmt;
    Pseudo::Expression* expr;
    Pseudo::VariableDeclaration* vardecl;
    Pseudo::ExpressionList* exprlist;
    Pseudo::VariableList* varlist;
    Pseudo::VectorAccess* vecaccess;
    Pseudo::Identifier* ident;
    Pseudo::String* pstring;
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
%token T_STRING
%token T_NEWLINE
%token T_OUTPUT
%token <token> T_ASSIGN T_SWAP
%token <token> T_EQ T_NOTEQ T_LESS T_LESSEQ T_GREATER T_GREATEREQ
%token <token> T_PLUS T_MINUS T_DIV T_MUL T_MODULUS
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
%type <stmt> stmt assign_stmt conditional loop_stmt return func_decl
%type <vardecl> var_decl

%type <varlist> func_decl_args
%type <exprlist> call_args
%type <token> comparison

%type <pstring> string

// exprs
%type <expr> expr numeric logical func_call matrix vector unaryop_expr boolean_expr binop_expr
%type <vecaccess> vector_access matrix_access
%type <ident> type_specifier ident

%left T_PLUS T_MINUS
%left T_MUL T_DIV T_MODULUS
%left T_AND T_NOT

//%left T_OR
//%left T_AND
////%left T_EQ T_NOTEQ
////%left T_LESS T_GREATER T_LESSEQ T_GREATEREQ
//%left T_PLUS T_MINUS
//%left T_MUL T_DIV T_MODULUS
//%precedence T_NOT

%start program

%%

program : %empty { driver.set_ast_root(new Pseudo::Block(@$)); printRule("program -> epsilon"); }
        | stmts { driver.set_ast_root($1); printRule("program -> stmts"); } 
        ;

stmts : stmt { $$ = new Pseudo::Block(@$); $$->statements.push_back($<stmt>1); printRule("stmts -> stmt"); }
      | stmts stmt { $1->statements.push_back($<stmt>2); printRule("stmts -> stmts stmt"); }
      ;

ident : T_IDENTIFIER { $$ = new Pseudo::Identifier(*$1, @$); printRule("ident"); }
      ;

stmt : var_decl close { printRule("stmts -> var_decl close"); }
     | func_decl close { printRule("stmts -> func_decl close"); }
     | assign_stmt close { printRule("stmts -> assign_stmt close"); }
     | expr close { printRule("stmts -> expr close"); $$ = new Pseudo::ExpressionStatement($1, @$); }
     | conditional close { printRule("stmts -> conditional close"); }
     | loop_stmt close { printRule("stmts -> loop_stmt close"); }
     | return close { printRule("stmts -> return close"); } 
     ;

func_decl : T_OUTPUT T_COLON type_specifier /*T_NEWLINE*/ T_FUNCTION ident T_LPAREN func_decl_args T_RPAREN close block T_FUNCTION T_END
            {
                //$$ = new FunctionDeclaration($3, $6, $8, $11);
                $$ = new FunctionDeclaration($3, $5, $7, $10, @$);
                printRule("func_decl -> T_OUTPUT T_COLON type_specifier T_FUNCTION ident T_LPAREN func_decl_args T_RPAREN block T_FUNCTION T_END");
            }
          ;

func_decl_args : %empty { $$ = new Pseudo::VariableList(); printRule("function_decl_args -> epsilon"); }
               | var_decl { $$ = new Pseudo::VariableList(); $$->push_back($1); printRule("func_decl_args -> var_decl"); }
               | func_decl_args T_COMMA var_decl { $1->push_back($3); printRule("funcition_decl_args -> func_decl_args T_COMMA var_decl"); }
               ;

//func_var_type_specifier : type_specifier { $$ = $1; }
//                        | T_ARRAY type_specifier { $$ = $1; }
//                        ;

var_decl :
    ident T_COLON type_specifier T_ASSIGN expr 
    {
        $$ = new Pseudo::VariableDeclaration($3, $1, $5, @$);
        printRule("var_decl -> ident T_COLON type_specifier T_ASSIGN expression");
    }
|   ident T_COLON type_specifier 
    {
        $$ = new Pseudo::VariableDeclaration($3, $1, @$);
        printRule("var_decl -> ident T_COLON type_specifier");
    }
;
	
assign_stmt:
    ident T_ASSIGN expr
    {
        $$ = new Pseudo::Assignment($<ident>1, $<expr>3, @$);
        printRule("assign_statement -> ident T_ASSIGN expr");
    }
|   
    vector_access T_ASSIGN expr
    {
        $$ = new Pseudo::ModifyVector($1, $3, @$);
        printRule("vector_access -> vector_access T_ASSIGN expr");
    }
|
    matrix_access T_ASSIGN expr
    {
        $$ = new Pseudo::ModifyVector($1, $3, @$);
        printRule("vector_access -> matrix_access T_ASSIGN expr");
    }
;

conditional:
    T_IF expr T_THEN close block T_IF_END T_END
    {
        $$ = new Conditional($2, $5, @$);
        printRule("conditional -> T_IF expr T_THEN block T_IF_END T_END");
    }
|
    T_IF expr T_THEN close block T_ELSE close block T_IF_END T_END
    {
        $$ = new Conditional($2, $5, $8, @$);
        printRule("conditional -> T_IF expr T_THEN block T_ELSE block T_IF_END T_END");
    }
;

loop_stmt:
    T_ITERATE T_WHILE expr close block T_ITERATE T_END
    {
        $$ = new WhileLoop($3, $5, @$);
        printRule("loop_stmt -> T_ITERATE T_WHILE expr close block T_ITERATE T_END");
    }
|
    T_ITERATE close block T_WHILE expr
    {
        $$ = new DoWhileLoop($5, $3, @$);
        printRule("loop_stmt -> T_ITERATE close block T_WHILE expr T_ITERATE T_END");
    }
|
    T_ITERATE ident T_ASSIGN expr T_MINUS T_FROM expr T_MINUS T_TO close block T_ITERATE T_END 
    {
        $$ = new ForLoop($2, $4, $7, $11, @$);
        printRule("loop_stmt -> T_ITERATE ident T_ASSIGN expr T_FROM expr T_TO close block T_ITERATE T_END");
    }
;

return:
    T_RETURN expr
    {
        $$ = new Return($2, @$);
        printRule("return -> T_RETURN expr");
    }
|
    T_RETURN
    {
        $$ = new Return(@$);
        printRule("return -> T_RETURN");
    }
;


block : T_INDENT stmts T_OUTDENT { $$ = $2; } 
      |   T_INDENT T_OUTDENT { $$ = new Pseudo::Block(@$); }
      ;

close : T_COMMA { printRule("close -> T_COMMA"); }
      | %empty { printRule("close -> epsilon"); }
      ;

//close : T_COMMA { printRule("close -> T_COMMA"); }
//      | T_NEWLINE { printRule("close -> T_NEWLINE"); }
//      | close T_NEWLINE { printRule("close -> close T_NEWLINE"); }
//      ;

expr : func_call { printRule("expr -> func_call"); }
     | ident { printRule("expr -> ident"); }
     | numeric { printRule("expr -> numeric"); }
     | logical { printRule("expr -> logical"); }
     | vector { printRule("expr -> vector"); }
     | matrix { printRule("expr -> matrix"); }
     | vector_access { printRule("expr -> vector_access"); }
     | matrix_access { printRule("expr -> matrix_access"); }
     | binop_expr { printRule("expr -> binop_expr"); }
     | boolean_expr { printRule("expr -> boolean_expr"); }
     | unaryop_expr { printRule("expr -> unaryop_expr"); }
     | string { printRule("expr -> string"); }
     | T_LPAREN expr T_RPAREN { $$ = $2; }
     ;

binop_expr : expr T_AND expr { $$ = new BinaryOperator($1, token::T_AND, $3, @$); printRule("expr -> expr T_AND expr"); }
           | expr T_OR expr { $$ = new BinaryOperator($1, token::T_OR, $3, @$); printRule("expr -> expr T_OR expr"); }
           | expr T_PLUS expr { $$ = new BinaryOperator($1, token::T_PLUS, $3, @$); printRule("expr -> expr T_PLUS expr"); }
           | expr T_MINUS expr { $$ = new BinaryOperator($1, token::T_MINUS, $3, @$); printRule("expr -> expr T_MINUS expr"); }
           | expr T_MUL expr { $$ = new BinaryOperator($1, token::T_MUL, $3, @$); printRule("expr -> expr T_MUL expr"); }
           | expr T_DIV expr { $$ = new BinaryOperator($1, token::T_DIV, $3, @$); printRule("expr -> expr T_DIV expr"); }
           | expr T_MODULUS expr { $$ = new BinaryOperator($1, token::T_MODULUS, $3, @$); printRule("expr -> expr T_MODULUS expr"); }
           ;

comparison : T_LESS { $$ = token::T_LESS; } 
           | T_GREATER { $$ = token::T_GREATER; printRule("comparison -> T_GREATER"); } 
           | T_LESSEQ  { $$ = token::T_LESSEQ; }
           | T_GREATEREQ { $$ = token::T_GREATEREQ; }
           | T_EQ { $$ = token::T_EQ; } 
           | T_NOTEQ { $$ = token::T_NOTEQ; }
           ;

boolean_expr : expr comparison expr { $$ = new CompareOperator($1, $2, $3, @$); printRule("boolean_expr -> expr comparison expr"); }
             ;

unaryop_expr : T_NOT expr { $$ = new UnaryOperator($1, $2, @$); printRule("unaryop_expr -> T_NOT expr"); }
             ;

func_call : ident T_LPAREN call_args T_RPAREN { $$ = new Pseudo::FunctionCall($1, $3, @$); printRule("func_call -> ident T_LPAREN call_args T_RPAREN"); }
          ;

call_args : %empty { $$ = new Pseudo::ExpressionList(); printRule("call_args -> epsilon"); }
          | expr { $$ = new Pseudo::ExpressionList(); $$->push_back($1); printRule("call_args -> expr"); }
          | call_args T_COMMA expr { $$->push_back($3); printRule("call_args -> call_args T_COMMA expr"); } 
          ;

numeric : T_INTEGER_VAL { $$ = new Pseudo::Integer($1, @$); printRule("numeric -> T_INTEGER_VAL"); }
        |   T_RATIONAL_VAL { $$ = new Pseudo::Rational($1, @$); printRule("numeric -> T_RATIONAL_VAL"); }
        ;

string : T_STRING_VAL { $$ = new Pseudo::String(*$1, @$); printRule("string -> T_STRING_VAL"); }
       ;

logical : T_TRUE { $$ = new Pseudo::Boolean(true, @$); printRule("logical -> T_TRUE"); }
        |   T_FALSE { $$ = new Pseudo::Boolean(false, @$); printRule("logical -> T_FALSE"); }
        ;

vector:
    // 'Letrehoz(egesz)[6]' 
    T_CREATEVECTOR T_LPAREN type_specifier T_RPAREN T_LSBRACKET expr T_RSBRACKET
    {
        // make sure type_spec is not void
        $$ = new Pseudo::Vector($3, $6, @$);
        printRule("vector -> T_CREATEVECTOR T_LPAREN type_specifier T_RPAREN T_LSBRACKET expr T_RSBACKET");
    }
;

vector_access:
    ident T_LSBRACKET expr T_RSBRACKET
    {
        $$ = new VectorAccess($1, $3, @$);
        printRule("vector_access -> ident T_LSBRACKET expr T_RSBRACKET");
    }
;

matrix:
    // 'TablaLetrehoz(egesz)[6, 5]'
    T_CREATEMATRIX T_LPAREN type_specifier T_RPAREN T_LSBRACKET expr T_COMMA expr T_RSBRACKET
    {
        auto indexExprs = new Pseudo::ExpressionList();
        indexExprs->push_back($6);
        indexExprs->push_back($8);
        $$ = new Pseudo::Vector($3, indexExprs, $6, @$);
        printRule("matrix -> T_CREATEMATRIX T_LPAREN type_specifier T_RPAREN T_LSBRACKET expr T_COMMA expr T_RSBACKET");
    }
;

matrix_access:
    ident T_LSBRACKET expr T_COMMA expr T_RSBRACKET
    {
        auto indexExprs = new ExpressionList();
        indexExprs->push_back($3);
        indexExprs->push_back($5);
        $$ = new Pseudo::VectorAccess($1, indexExprs, @$);
        printRule("matrix_access -> ident T_LSBRACKET expr T_COMMA expr T_RSBRACKET");
    }
;

type_specifier : T_INTEGER T_ARRAY { $$ = new Pseudo::Identifier("vec_integer", @$); printRule("type_specifier -> T_INTEGER T_ARRAY"); }
               | T_BOOLEAN T_ARRAY { $$ = new Pseudo::Identifier("vec_boolean", @$); printRule("type_specifier -> T_BOOLEAN T_ARRAY"); }
               | T_RATIONAL T_ARRAY { $$ = new Pseudo::Identifier("vec_rational", @$); printRule("type_specifier -> T_RATIONAL T_ARRAY"); }
               | T_BOOLEAN { $$ = new Pseudo::Identifier("boolean", @$); printRule("type_specifier -> T_BOOLEAN"); }
               | T_INTEGER { $$ = new Pseudo::Identifier("integer", @$); printRule("type_specifier -> T_INTEGER"); }
               | T_RATIONAL { $$ = new Pseudo::Identifier("rational", @$); printRule("type_specifier -> T_RATIONAL"); }
               | T_VOID { $$ = new Pseudo::Identifier("void", @$); printRule("type_specifier -> T_VOID"); }
               ;

%%

void Pseudo::Parser::error(const location& loc, const std::string& err_message)
{
    std::cerr << err_message << std::endl;
    std::cerr << "Ln " << loc.begin.line << ", Col " << loc.begin.column << std::endl;
}
