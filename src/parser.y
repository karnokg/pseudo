%language "c++"
%locations
%define api.value.type variant

%define parse.error verbose

%code provides {
    int yylex(yy::parser::semantic_type* yylval, yy::parser::location_type* yyalloc);
}

%{
    #include <memory>
    std::shared_ptr<N>  

    int teszt = 42; // the top level root node of our final AST
    
%}

%token T_PROGRAM
%token T_MODULUS
%token T_VOID
%token T_TRUE
%token T_FALSE
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
%token T_RETURN /* todo implement it*/
%token T_BOOLEAN
%token T_INTEGER
%token T_RATIONAL
%token T_ARRAY /* todo use it*/
%token T_PRINT
%token T_ASSIGN
%token T_SWAP /* todo use it*/
%token T_LESS
%token T_GREATER
%token T_LESSEQ
%token T_GREATEREQ
%token T_EQ
%token T_NOTEQ
%token T_ADD
%token T_MINUS
%token T_DIV
%token T_MUL
%token T_AND
%token T_OR
%token T_COMMA
%token T_DOT /* todo use it?*/
%token T_NOT
%token T_LPAREN
%token T_RPAREN
%token T_LSBRACKET
%token T_RSBRACKET
%token T_QUESTIONMARK /* todo use it*/
%token T_COLON
%token T_STRING
%token T_STRING_VAL
%token T_NEWLINE
%token T_IDENTIFIER
%token T_INTEGER_VAL /* todo use it */
%token T_RATIONAL_VAL /* todo use it */

%left T_OR
%left T_AND
%left T_EQ T_NOTEQ
%left T_LESS T_GREATER T_LESSEQ T_GREATEREQ
%left T_ADD T_MINUS
%left T_MUL T_DIV T_MODULUS
%precedence T_NOT

%%

start:
    stmts

func_decl :
    T_FUNCTION type_specifier T_IDENTIFIER T_LPAREN func_decl_args T_RPAREN T_NEWLINE stmts T_FUNCTION T_END
    {
        std::cout << "func_decl => type_specifier T_IDENTIFIER T_LPAREN func_decl_args T_RPAREN" << std::endl;

        
    }
;

func_decl_args :
    %empty 
    { 
        std::cout << "function_decl_args => empty" << std::endl;
    }
    | var_decl 
    {
        std::cout << "func_decl_args => var_decl" << std::endl;
    }
    | func_decl_args T_COMMA var_decl 
    {
        std::cout << "funcition_decl_args => func_decl_args T_COMMA var_decl" << std::endl;
    }
;

var_decl :
    T_IDENTIFIER T_COLON type_specifier T_ASSIGN expr 
    {
        std::cout << "var_decl => T_IDENTIFIER T_COLON type_specifier T_ASSIGN expression" << std::endl;
    }
|    T_IDENTIFIER T_COLON type_specifier 
    {
        std::cout << "var_decl => T_IDENTIFIER type_specifier T_IDENTIFIER" << std::endl;
    }
;

stmts:
    stmt
    {
        std::cout << "stmts -> stmt" << std::endl;
    }
|   stmts stmt
    {
        std::cout << "stmts -> stmts stmt" << std::endl;
    }

stmt:
    var_decl end_stmt
    {
        std::cout << "stmts => var_decl" << std::endl;
    }
    | func_decl end_stmt
    {
        std::cout << "stmts => func_decl" << std::endl;
    }
    | expr end_stmt
    {
        std::cout << "stmts => expr" << std::endl;
    }
;

end_stmt:
    T_COMMA
    | T_NEWLINE

expr:
    assign_stmt end_stmt 
|   func_call 
|   T_IDENTIFIER
    {
    }
|   numeric 
    {

    }
|   logical
    {
        std::cout << "expr -> logical" << std::endl;
    }
|   expr T_ADD expr
    {
        std::cout << "expr -> expr T_ADD expr" << std::endl;
    }
|
    expr T_MINUS expr
    {
        std::cout << "expr -> expr T_MINUS expr" << std::endl;
    }
|
    expr T_MUL expr
    {
        std::cout << "expr -> expr T_MUL expr" << std::endl;
    }
|
    expr T_DIV expr
    {
        std::cout << "expr -> expr T_DIV expr" << std::endl;
    }
|
    expr T_MODULUS expr
    {
        std::cout << "expr -> expr T_MODULUS expr" << std::endl;
    }
|
    expr T_LESS expr
    {
        std::cout << "expr -> expr T_LESS expr" << std::endl;
    }
|
    expr T_GREATER expr
    {
        std::cout << "expr -> expr T_GREATER expr" << std::endl;
    }
|
    expr T_LESSEQ expr
    {
        std::cout << "expr -> expr T_LESSEQ expr" << std::endl;
    }
|
    expr T_GREATEREQ expr
    {
        std::cout << "expr -> expr T_GREATEREQ expr" << std::endl;
    }
|
    expr T_AND expr
    {
        std::cout << "expr -> expr T_AND expr" << std::endl;
    }
|
    expr T_OR expr
    {
        std::cout << "expr -> expr T_OR expr" << std::endl;
    }
|
    expr T_EQ expr
    {
        std::cout << "expr -> expr T_EQ expr" << std::endl;
    }
|
    expr T_NOTEQ expr
    {
        std::cout << "expr -> expr T_NOTEQ expr" << std::endl;
    }
|
    T_NOT expr
    {
        std::cout << "expr -> T_NOT expr" << std::endl;
    }
|   T_LPAREN expr T_RPAREN
    {
    }
	
assign_stmt:
    T_IDENTIFIER T_ASSIGN expr
    {
        std::cout << "assign_statement -> T_IDENTIFIER T_ASSIGN expr" << std::endl;
    }
;

func_call:
    T_IDENTIFIER T_LPAREN call_args T_RPAREN
    {
        std::cout << "func_call => T_IDENTIFIER T_LPAREN call_args T_RPAREN" << std::endl;
    }
;

call_args:
    %empty
    { 
        std::cout << "call_args => epsilon" << std::endl;
    }
    | expr 
    {
        std::cout << "call_args => expr" << std::endl;
    }
    | call_args T_COMMA expr 
    {
        std::cout << "call_args => call_args T_COMMA expr" << std::endl;
    } 
;

numeric:
    T_INTEGER_VAL
    {
        std::cout << "numeric => T_INTEGER_VAL" << std::endl;
    }
|   T_RATIONAL_VAL
    {
        std::cout << "numeric => T_RATIONAL_VAL" << std::endl;
    }
;

logical:
    T_TRUE
    {
        std::cout << "logical -> T_TRUE" << std::endl;
    }
|   T_FALSE
    {
        std::cout << "logical -> T_FALSE" << std::endl;
    }

type_specifier:
    T_BOOLEAN
    {
        std::cout << "type_specifier -> T_BOOLEAN" << std::endl;
    }
|   T_INTEGER
    {
        std::cout << "type_specifier -> T_INTEGER" << std::endl;
    }
|   T_RATIONAL
    {
        std::cout << "type_specifier -> T_RATIONAL" << std::endl;
    }
|   T_STRING
    {
        std::cout << "type_specifier -> T_STRING" << std::endl;
    }
|   T_VOID
    {
        std::cout << "type_specifier -> T_VOID" << std::endl;
    }
;

/*
raw_statement:
    if_statement
    {
        std::cout << "raw_statement -> if_statement" << std::endl;
    }
|   declaration_statement
    {
        std::cout << "raw_statement -> declaration_statement" << std::endl;
    }
|   assign_statement
    {
        std::cout << "raw_statement -> assign_statement" << std::endl;
    }
|   print_statement
    {
        std::cout << "raw_statement -> print_statement" << std::endl;
    }
|   iterator_statement
    {
        std::cout << "raw_statement -> iterator_statement" << std::endl;
    }
|   return_statement 
    {
        std::cout << "raw_statement -> return_statement" << std::endl;
    }
;

if_statement:
    T_IF expression T_THEN statements T_IF_END T_END
    {
        std::cout << "if_statement -> T_IF expression T_THEN statements T_IF_END T_END" << std::endl;
    }
|   T_IF expression T_THEN statements T_ELSE statements T_IF_END T_END
    {
        std::cout << "if_statement -> T_IF expression T_THEN statements T_ELSE statements T_IF_END T_END" << std::endl;
    }
;

declaration_statement:
    type T_IDENTIFIER 
    {
        std::cout << "declaration_statement -> type T_IDENTIFIER" << std::endl;
    }
|   type T_IDENTIFIER T_ASSIGN expression
    {
        std::cout << "declaration_statement -> type T_IDENTIFIER T_ASSIGN expression" << std::endl;
    }
;

print_statement:
    T_PRINT T_LPAREN T_STRING_VAL T_RPAREN
    {
        std::cout << "print_statement -> T_PRINT T_LPAREN T_STRING_VAL T_RPAREN" << std::endl;
    }
;

iterator_statement:
    T_ITERATE T_WHILE expression statements T_ITERATE T_END                             // ciklus amig boolean_expression statements ciklus vege
    {
        std::cout << "iterator_statement -> T_ITERATE T_WHILE expression statements T_ITERATE T_END" << std::endl;
    }
|   T_ITERATE T_WHILE T_LPAREN expression T_RPAREN statements T_ITERATE T_END         // ciklus amig (boolean_expression) statements ciklus vege todo: 2 esetbol 1
    {
        std::cout << "iterator_statement -> T_ITERATE T_WHILE T_LPAREN expression T_RPAREN statements T_ITERATE T_END" << std::endl;
    }
|   T_ITERATE statements T_WHILE expression                                           // ciklus statements amig expression
    {
        std::cout << "iterator_statement -> T_ITERATE statements T_WHILE expression" << std::endl;
    }
|   T_ITERATE statements T_WHILE T_LPAREN expression T_RPAREN                         // ciklus statements amig (expression) todo: 2 esetbol 1
    {
        std::cout << "iterator_statement -> T_ITERATE statements T_WHILE T_LPAREN expression T_RPAREN" << std::endl;
    }
|   T_ITERATE T_IDENTIFIER T_ASSIGN iterator_range statements T_ITERATE T_END         // ciklus (existing_identifier) <- iterator_range
    {
        std::cout << "iterator_statement -> T_ITERATE T_IDENTIFIER T_ASSIGN iterator_range statements T_ITERATE T_END" << std::endl;
    }
|   T_ITERATE  T_ASSIGN iterator_range statements T_ITERATE T_END                     // ciklus (new_identifier) <- iterator_range check existing
    {
        std::cout << "iterator_statement -> T_ITERATE  T_ASSIGN iterator_range statements T_ITERATE T_END" << std::endl;
    }
;

iterator_range:
    expression T_FROM expression T_TO                                                   // numeric_expression
    {
        std::cout << "iterator_range -> expression T_FROM expression T_TO" << std::endl;
    }
;

return_statement: /* we do not need end_statement, cause it doesnt matter what's after the return*
    T_RETURN T_NEWLINE
    {
        std::cout << "return_statement -> T_RETURN";
    }
|   T_RETURN expression T_NEWLINE
    {
        std::cout << "return_statement -> T_RETURN expression";
    }
;

call:
    T_IDENTIFIER T_LPAREN function_call_arguments T_RPAREN
    {
        std::cout << "call -> T_IDENTIFIER T_LPAREN function_call_arguments T_RPAREN" << std::endl;
    }
;

function_call_arguments:
     %empty {
        //%empty
        std::cout << "function_call_arguments -> epsilon" << std::endl;
    }
|    function_call_argument_list
    {
        std::cout << "function_call_arguments -> function_call_argument_list" << std::endl;
    }
;

function_call_argument_list:
    function_call_argument_list T_COMMA expression
    {
        std::cout << "function_call_argument_list -> function_call_argument_list T_COMMA expression" << std::endl;
    }
|   expression
    {
        std::cout << "function_call_argument_list -> expression" << std::endl;
    }
;

expression:
    T_RATIONAL_VAL
    {
        std::cout << "expression -> T_RATIONAL_VAL" << std::endl;
    }
|
    T_INTEGER_VAL
    {
        std::cout << "expression -> T_INTEGER_VAL" << std::endl;
    }
|
    T_TRUE
    {
        std::cout << "expression -> T_TRUE" << std::endl;
    }
|
    T_FALSE
    {
        std::cout << "expression -> T_FALSE" << std::endl;
    }
|
    T_IDENTIFIER
    {
        std::cout << "expression -> T_IDENTIFIER" << std::endl;
    }
|
    expression T_PLUS expression
    {
        std::cout << "expression -> expression T_PLUS expression" << std::endl;
    }
|
    expression T_MINUS expression
    {
        std::cout << "expression -> expression T_MINUS expression" << std::endl;
    }
|
    expression T_MULTIPLY expression
    {
        std::cout << "expression -> expression T_MULTIPLY expression" << std::endl;
    }
|
    expression T_DIVIDE expression
    {
        std::cout << "expression -> expression T_DIVIDE expression" << std::endl;
    }
|
    expression T_MODULUS expression
    {
        std::cout << "expression -> expression T_MODULUS expression" << std::endl;
    }
|
    expression T_LESSTHAN expression
    {
        std::cout << "expression -> expression T_LESSTHAN expression" << std::endl;
    }
|
    expression T_GREATERTHAN expression
    {
        std::cout << "expression -> expression T_GREATER expression" << std::endl;
    }
|
    expression T_LESSTHANEQUALS expression
    {
        std::cout << "expression -> expression T_LESSEQ expression" << std::endl;
    }
|
    expression T_GREATERTHANEQUALS expression
    {
        std::cout << "expression -> expression T_GREATEREQ expression" << std::endl;
    }
|
    expression T_AND expression
    {
        std::cout << "expression -> expression T_AND expression" << std::endl;
    }
|
    expression T_OR expression
    {
        std::cout << "expression -> expression T_OR expression" << std::endl;
    }
|
    expression T_EQUALS expression
    {
        std::cout << "expression -> expression T_EQ expression" << std::endl;
    }
|
    T_NOT expression
    {
        std::cout << "expression -> T_NOT expression" << std::endl;
    }*/
%%

