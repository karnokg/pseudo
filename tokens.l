%{
    #include <string>
    #include "AST.h"
    #include "parser.hpp"
    
    #define SAVE_TOKEN yylval.string = new std::string(yytext, yyleng)
    #define TOKEN(t) (yylval.token = t)

    extern "C" int yywrap() { }
%}

%%

[ \t\n]                         ;
[a-zA-Z_][a-zA-Z0-9_]*          SAVE_TOKEN; return TOK_IDENTIFIER;
[0-9]+                          SAVE_TOKEN; return TOK_INTEGER_NUM;
[0-9]+.[0-9]*                   SAVE_TOKEN; return TOK_RATIONAL_NUM;
"<-"                            TOKEN(TOK_ASSIGN);
"<->"                           TOKEN(TOK_SWAP);
"<"                             TOKEN(TOK_LESSTHAN);
">"                             TOKEN(TOK_GREATERTHAN);
"<="                            TOKEN(TOK_LESSTHANEQUALS);
">="                            TOKEN(TOK_GREATERTHANEQUALS);
"="                             TOKEN(TOK_EQUALS);
"=/="                           TOKEN(TOK_NOTEQUALS);
"+"                             TOKEN(TOK_PLUS);
"-"                             TOKEN(TOK_MINUS);
"/"                             TOKEN(TOK_DIVIDE);
"*"                             TOKEN(TOK_MULTIPLY);
"mod"                           TOKEN(TOK_MODULUS);
"&&"                            TOKEN(TOK_AND);
"||"                            TOKEN(TOK_OR);
","                             TOKEN(TOK_COMMA);
"."                             TOKEN(TOK_DOT);
"!"                             TOKEN(TOK_NOT);
"igaz"                          TOKEN(TOK_TRUE);
"hamis"                         TOKEN(TOK_FALSE);
"("                             TOKEN(TOK_LPAREN);
")"                             TOKEN(TOK_RPAREN);
"függvény"                      TOKEN(TOK_FUNCTION);
"vége"                          TOKEN(TOK_END);
"ha"                            TOKEN(TOK_IF);
"különben"                      TOKEN(TOK_ELSE);
"akkor"                         TOKEN(TOK_THEN);
"elágazás"                      TOKEN(TOK_IF_END);
"ciklus"                        TOKEN(TOK_ITERATE);
"tól"                           TOKEN(TOK_FROM);
"től"                           TOKEN(TOK_FROM);
"ig"                            TOKEN(TOK_TO);
"vissza"                        TOKEN(TOK_RETURN);
.                               printf("unknown token"); yyterminate();

%%
