#include <iostream>
#include <fstream>
//include "parser.tab.hh"
#include "parser.h"
#include "FlexLexer.h"

yyFlexLexer* lexer;

extern int teszt;

void yy::parser::error(const location_type& loc, const std::string& msg) {
    std::cerr << "Syntax error: " << loc.begin.line << ":" << loc.begin.column << ": " << msg << std::endl;
    exit(1);
}

int yylex(yy::parser::semantic_type* yylval, yy::parser::location_type* yylloc) 
{
    yylloc->begin.line = lexer->lineno();

    int token = lexer->yylex();
    if(token == yy::parser::token::T_IDENTIFIER)
     {
        yylval->build(std::string(lexer->YYText()));
    } else if (token == yy::parser::token::T_STRING)
    {
        yylval->build(std::string(lexer->YYText()));
    }

    return token;
}

int main(int argc, char* argv[])
{
    std::cout << teszt;
    if (argc != 2)
    {
        std::cout << "no input" << std::endl;
        return 1;
    }

    std::ifstream input(argv[1]);
    if (!input)
    {
        std::cout << "can't open file" << std::endl;
        return 1;
    }

    lexer = new yyFlexLexer(&input);
// LEXER DEBUG

    for (int i = 0; i < 1000; ++i)
    {
        lexer->yylex();
    }

    return 0;
// DEBUG_END

    yy::parser parser;
    int result = parser.parse();

    delete lexer;
    return result;
}
