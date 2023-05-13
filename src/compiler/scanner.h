#ifndef H_SCANNER
#define H_SCANNER

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "parser.h"
#include "location.hh"

class Interpreter;

namespace Pseudo
{
    
class Scanner : public yyFlexLexer
{
public:
    Scanner(std::istream* in) : yyFlexLexer(in)
    {
        //yyloc = new location(position(nullptr, 0, 0), position(nullptr, 0, 0));
    }

    virtual ~Scanner() {}

    using FlexLexer::yylex;

//    virtual int yylex(Pseudo::Parser::semantic_type* const lval, Pseudo::Parser::location_type* location);
    virtual int yylex(Pseudo::Parser::semantic_type* const lval, location* yyloc);

private:
    Pseudo::Parser::semantic_type* yylval{nullptr};
    //location* yyloc{nullptr};
};

}
#endif