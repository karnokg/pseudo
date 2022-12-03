%option c++ noyywrap yylineno 

%{
#include <iostream>
#include "parser.tab.hh"

int col = 0;

#define YY_USER_ACTION \
    col += YYLeng();

#define PSEUDO_LEXER_LOG_ENABLED 1 

#define PRINT_TOKEN(STR) debug_token(STR, YYText())

void debug_token(const std::string& tokenName, const std::string& yyText)
{
    if (PSEUDO_LEXER_LOG_ENABLED)
    {
        std::string cleanedYYText = yyText;
        if (cleanedYYText == "\n")
        {
            cleanedYYText = "\\n";
        }

        std::cout << tokenName << "\t" << cleanedYYText << std::endl;
    }
}

%}

%%
program {
    PRINT_TOKEN("T_PROGRAM");
    return yy::parser::token::T_PROGRAM;
}

mod {
    PRINT_TOKEN("T_MODULUS");
    return yy::parser::token::T_MODULUS;
}

igaz {
    PRINT_TOKEN("T_TRUE");
    return yy::parser::token::T_TRUE;
}

hamis {
    PRINT_TOKEN("T_PROGRAM");
    return yy::parser::token::T_FALSE;
}

fuggveny {
    PRINT_TOKEN("T_PROGRAM");
    return yy::parser::token::T_FUNCTION;
}

vege {
    PRINT_TOKEN("T_END");
    return yy::parser::token::T_END;
}

ha {
    PRINT_TOKEN("T_IF");
    return yy::parser::token::T_IF;
}

kulonben {
    PRINT_TOKEN("T_ELSE");
    return yy::parser::token::T_ELSE;
}

akkor {
    PRINT_TOKEN("T_THEN");
    return yy::parser::token::T_THEN;
}

elagazas {
    PRINT_TOKEN("T_IF_END");
    return yy::parser::token::T_IF_END;
}

ciklus {
    PRINT_TOKEN("T_ITERATE");
    return yy::parser::token::T_ITERATE;
}

amig {
    PRINT_TOKEN("T_WHILE");
    return yy::parser::token::T_WHILE;
}

tol {
    PRINT_TOKEN("T_FROM");
    return yy::parser::token::T_FROM;
}

ig {
    PRINT_TOKEN("T_TO");
    return yy::parser::token::T_TO;
}

vissza {
    PRINT_TOKEN("T_RETURN");
    return yy::parser::token::T_RETURN;
}

logikai {
    PRINT_TOKEN("T_BOOLEAN");
    return yy::parser::token::T_BOOLEAN;
}

egesz {
    PRINT_TOKEN("T_INTEGER");
    return yy::parser::token::T_INTEGER;
}

racionalis {
    PRINT_TOKEN("T_RATIONAL");
    return yy::parser::token::T_RATIONAL;
}

tomb {
    PRINT_TOKEN("T_ARRAY");
    return yy::parser::token::T_ARRAY;
}

kiir {
    PRINT_TOKEN("T_PRINT");
    return yy::parser::token::T_PRINT;
}

szoveg {
    PRINT_TOKEN("T_STRING");
    return yy::parser::token::T_STRING;
}

void {
    PRINT_TOKEN("T_VOID");
    return yy::parser::token::T_VOID;
}

"<-" {
    PRINT_TOKEN("T_ASSIGN");
    return yy::parser::token::T_ASSIGN;
}

"<->" {
    PRINT_TOKEN("T_SWAP");
    return yy::parser::token::T_SWAP;
}

"<" {
    PRINT_TOKEN("T_LESS");
    return yy::parser::token::T_LESS;
}

">" {
    PRINT_TOKEN("T_GREATER");
    return yy::parser::token::T_GREATER;
}

"<=" {
    PRINT_TOKEN("T_LESSEQ");
    return yy::parser::token::T_LESSEQ;
}

">=" {
    PRINT_TOKEN("T_GREATEREQ");
    return yy::parser::token::T_GREATEREQ;
}

"=" {
    PRINT_TOKEN("T_EQ");
    return yy::parser::token::T_EQ;
}

"=/=" {
    PRINT_TOKEN("T_NOTEQ");
    return yy::parser::token::T_NOTEQ;
}

"+" {
    PRINT_TOKEN("T_ADD");
    return yy::parser::token::T_ADD;
}

"-" {
    PRINT_TOKEN("T_MINUS");
    return yy::parser::token::T_MINUS;
}

"/" {
    PRINT_TOKEN("T_DIV");
    return yy::parser::token::T_DIV;
}

"*" {
    PRINT_TOKEN("T_MUL");
    return yy::parser::token::T_MUL;
}

"&&" {
    PRINT_TOKEN("T_AND");
    return yy::parser::token::T_AND;
}

"||" {
    PRINT_TOKEN("T_OR");
    return yy::parser::token::T_OR;
}

"," {
    PRINT_TOKEN("T_COMMA");
    return yy::parser::token::T_COMMA;
}

"." {
    PRINT_TOKEN("T_DOT");
    return yy::parser::token::T_DOT;
}

"!" {
    PRINT_TOKEN("T_NOT");
    return yy::parser::token::T_NOT;
}

"(" {
    PRINT_TOKEN("T_LPAREN");
    return yy::parser::token::T_LPAREN;
}

")" {
    PRINT_TOKEN("T_RPAREN");
    return yy::parser::token::T_RPAREN;
}

"[" {
    PRINT_TOKEN("T_LSBRACKET");
    return yy::parser::token::T_LSBRACKET;
}

"]" {
    PRINT_TOKEN("T_RSBRACKET");
    return yy::parser::token::T_RSBRACKET;
}

"?" {
    PRINT_TOKEN("T_QUESTIONMARK");
    return yy::parser::token::T_QUESTIONMARK;
}

":" {
    PRINT_TOKEN("T_COLON");
    return yy::parser::token::T_COLON;
}

\"([^\\\"]|\\.)*\" {

    PRINT_TOKEN("T_STRING_VAL");
    return yy::parser::token::T_STRING_VAL;
}

[ \t] {
    ;
}

[\n]* {
    col = 0;
    PRINT_TOKEN("T_NEWLINE");
    return yy::parser::token::T_NEWLINE;
}

[a-zA-Z_][a-zA-Z0-9_]* {
    PRINT_TOKEN("T_IDENTIFIER");
    return yy::parser::token::T_IDENTIFIER;
}

[0-9]+ {
    PRINT_TOKEN("T_INTEGER_VAL");
    return yy::parser::token::T_INTEGER_VAL;
}

[0-9]+\.[0-9]* {
    PRINT_TOKEN("T_RATIONAL_VAL");
    return yy::parser::token::T_RATIONAL_VAL;
}

. {
    std::cout << "unexpected character '" << YYText() << "' at '" << lineno() << ":" << col << "'" << std::endl;
    //exit(1);
}
