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
        loc = new Pseudo::Parser::location_type();
    }

    virtual ~Scanner() {}

    using FlexLexer::yylex;

    virtual int yylex(Pseudo::Parser::semantic_type* const lval,
            Pseudo::Parser::location_type* location);
private:
    Pseudo::Parser::semantic_type* yylval = nullptr;
    Pseudo::Parser::location_type* loc = nullptr;
};

}
#endif