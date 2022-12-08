#include <iostream>
#include <fstream>
#include "interpreter.h"
/*
int yylex(yy::parser::semantic_type* yylval, yy::parser::location_type* yylloc)
{
    yylloc->begin.line = lexer->lineno();

    int token = lexer->yylex();
    if(token == yy::parser::token::T_IDENTIFIER)
     {
        yylval->string = new std::string(lexer->YYText());
    } else if (token == yy::parser::token::T_STRING)
    {
        yylval->string = new std::string(lexer->YYText());
    }

    return token;
    }

void yy::parser::error(const location_type& loc, const std::string& msg) {
    std::cerr << "Line " << loc.begin.line << ": " << msg << std::endl;
    exit(1);
}
*/

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "no input" << std::endl;
        return 1;
    }

    std::string file(argv[1]);

    Pseudo::Interpreter driver;
    int result = driver.parse(file);

    std::cout << driver.get_ast_root()->statements.size();

    return result;
}
