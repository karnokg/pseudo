#include <iostream>
#include <fstream>
#include <vector>
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
    if (argc < 2)
    {
        std::cout << "No file input!" << std::endl;
        return 1;
    }

    std::string filename(argv[1]);
    //std::cout << "filename=" << filename << std::endl;
    bool run = false;
    bool verbose = false;
    bool silent = false;

    //std::cout << "Pseudo inputs:" << std::endl;
    for (int i = 1; i < argc; ++i)
    {
        std::string currArg(argv[i]);
        if (currArg == "--run" || currArg == "-r")
        {
            run = true;
        }
        else if (currArg == "--verbose" || currArg == "-v")
        {
            verbose = true;
        }
        else if (currArg == "--silent" || currArg == "-s")
        {
            silent = true;
        }

        //std::cout << "argv[" << i << "] " << currArg << std::endl; 
    }

    Pseudo::Interpreter driver(filename);
    bool parsingSucceeded = driver.parse();

    if (parsingSucceeded)
    {
        driver.generateCode(verbose, run, silent);
    }

    return 0;
}
