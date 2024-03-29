%option c++ 
/*%option debug*/
%option yyclass="Pseudo::Scanner"
%option noyywrap 
%option nodefault
/*%option yylineno*/

%{
#include <iostream>
#include <stack>
#include <string>
#include "scanner.h"
#include "location.hh"

#undef YY_DECL
#define YY_DECL int Pseudo::Scanner::yylex(Pseudo::Parser::semantic_type* const lval, Pseudo::location* yyloc)
//#define YY_DECL int Pseudo::Scanner::yylex(Pseudo::Parser::semantic_type* const lval, location_type* location)

#define PSEUDO_LEXER_LOG_ENABLED 0 

using token = Pseudo::Parser::token;

int g_current_line_indent = 0;
std::stack<size_t> g_indent_levels;
int g_is_fake_outdent_symbol = 0;

int location_ignore_new_line = 0;
static void updateLoc(Pseudo::location* yyloc, const char* txt)
{
	auto s = std::string(txt);
    {
        size_t pos;
        while ((pos = s.find('\n')) != std::string::npos) {
            s.replace(pos, 1, "`\\n`");
        }
    }

	yyloc->begin.line = yyloc->end.line;
	yyloc->begin.column = yyloc->end.column;
    for (int i = 0; txt[i] != '\0'; ++i)
    {
        if (txt[i] == '\n')
        {
            if (!location_ignore_new_line)
            {
                yyloc->end.line++;
                yyloc->end.column = 0;
            }
            else if (PSEUDO_LEXER_LOG_ENABLED)
            {
                std::cout << "# ignored new line" << std::endl;
            }
        } 
        else
        { 
            yyloc->end.column++; 
        }
    }

    if (PSEUDO_LEXER_LOG_ENABLED)
    {
        std::cout << "buffer: `" << s << "` | ";
        std::cout << "begin line " << yyloc->begin.line << " column " << yyloc->begin.column << " | ";
        std::cout << "end line " << yyloc->end.line << " column " << yyloc->end.column << std::endl;
    }
}

//#define yyterminate() return (token::T_ENDOFFILE)
#define yyterminate() return YY_NULL

#define YY_NO_UNISTD_H

#define YY_USER_ACTION updateLoc(yyloc, YYText());

#define PRINT_TOKEN(STR) debug_token(STR, YYText())

#define YY_USER_INIT { \
    g_indent_levels.push(0); \
    BEGIN(initial); \
}

    int col = 1;
    int yycolumn = 1;

    void debug_token(const std::string& tokenName, const std::string& yyText)
    {
        if (PSEUDO_LEXER_LOG_ENABLED)
        {
            std::string cleanedYYText = yyText;
            if (cleanedYYText == "\n")
            {
                cleanedYYText = "\\n";
            }

            for (int i = 0; i < g_indent_levels.top(); ++i)
            {
                std::cout << "\t";
            } 
            std::cout << tokenName << "\t" << cleanedYYText << std::endl;
        }
    }
%}

%x initial
%x indent
%s normal

%%
%{
    int indent_caller = normal;
    yylval = lval;
%}

<initial>. {
    yycolumn -= 1;
    indent_caller = normal;
    yyless(0);
    BEGIN(indent); 
}

<indent>#.*[\n] {
    /* ignored */
}

<initial>\n {
    indent_caller = normal;
    yyless(0);
    //std::cerr << "Lexical Analyis: There are new line characters `\\n` on the top of the file, please remove them." << std::endl;
    //exit(1);
    BEGIN(indent);
}   

<indent>" " {
    std::cout << "Lexical Analysis: You used space based indentation, which is not supported." << std::endl;
    std::cout << "Ln " << yyloc->begin.line << ", Col " << yyloc->begin.column << std::endl;
    exit(1);
}

<indent>\t {
    ++g_current_line_indent;
}

<indent>\n { 
    location_ignore_new_line = 0;
    g_current_line_indent = 0;
//    PRINT_TOKEN("T_NEWLINE");
//    return token::T_NEWLINE;
}

<<EOF>> {
    if (g_indent_levels.top() != 0) 
    {
        g_indent_levels.pop();

        if (g_current_line_indent != g_indent_levels.top()) 
        {
            unput('\n');
            for (size_t i = 0 ; i < g_indent_levels.top() ; ++i) 
            {
                unput(' ');
            }
        }
        else
        {
            BEGIN(indent_caller);
        }

        PRINT_TOKEN("T_OUTDENT");
        return token::T_OUTDENT;
    }
    else
    {
        yyterminate();
    }
}

<indent>. {
    if(!g_is_fake_outdent_symbol) {
        unput(*yytext);
    }

    g_is_fake_outdent_symbol = 0;

    // -2: -1 for putting it back and -1 for ending at the last space.
    yycolumn -= 1;

    // Indentation level has increased.
    if (g_current_line_indent > g_indent_levels.top()) 
    {
        g_indent_levels.push(g_current_line_indent);
        BEGIN(indent_caller);

        PRINT_TOKEN("T_INDENT");
        return token::T_INDENT;
    }
    else if (g_current_line_indent < g_indent_levels.top()) 
    {
        // Outdenting is the most difficult, as we might need to
        // outdent multiple times at once, but flex doesn't allow
        // emitting multiple tokens at once! So we fake this by
        // 'unput'ting fake lines which will give us the next
        // outdent.
        g_indent_levels.pop();

        if (g_current_line_indent != g_indent_levels.top()) 
        {
            // Unput the rest of the current line, including the newline.
            // We want to keep it untouched.
            for (size_t i = 0 ; i < g_current_line_indent ; ++i) {
                unput(' ');
            }
            unput('\n');
            // Now, insert a fake character indented just so
            // that we get a correct outdent the next time.
            unput('.');
            // Though we need to remember that it's a fake one
            // so we can ignore the symbol.
            g_is_fake_outdent_symbol = 1;

            for (size_t i = 0 ; i < g_indent_levels.top() ; ++i) 
            {
                unput(' ');
            }

            unput('\n');
        }
        else 
        {
            BEGIN(indent_caller);
        }

        PRINT_TOKEN("T_OUTDENT");
        return token::T_OUTDENT;
    }
    else
    {
        // No change in indentation, not much to do here...
        BEGIN(indent_caller);
    }
}

<normal>\n {
    g_current_line_indent = 0;
    indent_caller = YY_START;
    unput('\n'); /* let indent mode handle new lines! */
    location_ignore_new_line = 1;
    BEGIN(indent); 
}

#.* {
    /* ignored */
}

\r {
    /* ignored */
}
program {
    PRINT_TOKEN("T_PROGRAM");
    return token::T_PROGRAM;
}

Kimenet {
    PRINT_TOKEN("T_OUTPUT");
    return token::T_OUTPUT;
}

mod {
    PRINT_TOKEN("T_MODULUS");
    return token::T_MODULUS;
}

igaz {
    PRINT_TOKEN("T_TRUE");
    return token::T_TRUE;
}

hamis {
    PRINT_TOKEN("T_PROGRAM");
    return token::T_FALSE;
}

fuggveny {
    PRINT_TOKEN("T_FUNCTION");
    return token::T_FUNCTION;
}

vege {
    PRINT_TOKEN("T_END");
    return token::T_END;
}

ha {
    PRINT_TOKEN("T_IF");
    return token::T_IF;
}

kulonben {
    PRINT_TOKEN("T_ELSE");
    return token::T_ELSE;
}

akkor {
    PRINT_TOKEN("T_THEN");
    return token::T_THEN;
}

elagazas {
    PRINT_TOKEN("T_IF_END");
    return token::T_IF_END;
}

ciklus {
    PRINT_TOKEN("T_ITERATE");
    return token::T_ITERATE;
}

amig {
    PRINT_TOKEN("T_WHILE");
    return token::T_WHILE;
}

tol {
    PRINT_TOKEN("T_FROM");
    return token::T_FROM;
}

ig {
    PRINT_TOKEN("T_TO");
    return token::T_TO;
}

vissza {
    PRINT_TOKEN("T_RETURN");
    return token::T_RETURN;
}

logikai {
    PRINT_TOKEN("T_BOOLEAN");
    return token::T_BOOLEAN;
}

egesz {
    PRINT_TOKEN("T_INTEGER");
    return token::T_INTEGER;
}

racionalis {
    PRINT_TOKEN("T_RATIONAL");
    return token::T_RATIONAL;
}

tomb {
    PRINT_TOKEN("T_ARRAY");
    return token::T_ARRAY;
}

szoveg {
    PRINT_TOKEN("T_STRING");
    return token::T_STRING;
}

nincs {
    PRINT_TOKEN("T_VOID");
    return token::T_VOID;
}

Letrehoz {
    PRINT_TOKEN("T_CREATEVECTOR");
    return token::T_CREATEVECTOR;
}

TablaLetrehoz {
    PRINT_TOKEN("T_CREATEMATRIX");
    return token::T_CREATEMATRIX;
}

"<-" {
    PRINT_TOKEN("T_ASSIGN");
    return token::T_ASSIGN;
}

"<->" {
    PRINT_TOKEN("T_SWAP");
    return token::T_SWAP;
}

"<" {
    PRINT_TOKEN("T_LESS");
    return token::T_LESS;
}

">" {
    PRINT_TOKEN("T_GREATER");
    return token::T_GREATER;
}

"<=" {
    PRINT_TOKEN("T_LESSEQ");
    return token::T_LESSEQ;
}

">=" {
    PRINT_TOKEN("T_GREATEREQ");
    return token::T_GREATEREQ;
}

"=" {
    PRINT_TOKEN("T_EQ");
    return token::T_EQ;
}

"=/=" {
    PRINT_TOKEN("T_NOTEQ");
    return token::T_NOTEQ;
}

"+" {
    PRINT_TOKEN("T_PLUS");
    return token::T_PLUS;
}

"-" {
    PRINT_TOKEN("T_MINUS");
    return token::T_MINUS;
}

"/" {
    PRINT_TOKEN("T_DIV");
    return token::T_DIV;
}

"*" {
    PRINT_TOKEN("T_MUL");
    return token::T_MUL;
}

"&&" {
    PRINT_TOKEN("T_AND");
    return token::T_AND;
}

"||" {
    PRINT_TOKEN("T_OR");
    return token::T_OR;
}

"," {
    PRINT_TOKEN("T_COMMA");
    return token::T_COMMA;
}

"." {
    PRINT_TOKEN("T_DOT");
    return token::T_DOT;
}

"!" {
    PRINT_TOKEN("T_NOT");
    return token::T_NOT;
}

"(" {
    PRINT_TOKEN("T_LPAREN");
    return token::T_LPAREN;
}

")" {
    PRINT_TOKEN("T_RPAREN");
    return token::T_RPAREN;
}

"[" {
    PRINT_TOKEN("T_LSBRACKET");
    return token::T_LSBRACKET;
}

"]" {
    PRINT_TOKEN("T_RSBRACKET");
    return token::T_RSBRACKET;
}

"?" {
    PRINT_TOKEN("T_QUESTIONMARK");
    return token::T_QUESTIONMARK;
}

":" {
    PRINT_TOKEN("T_COLON");
    return token::T_COLON;
}

\"([^\\\"]|\\.)*\" {

    PRINT_TOKEN("T_STRING_VAL");
    yylval->string = new std::string(YYText(), YYLeng());
    return token::T_STRING_VAL;
}

[ \t] {
    ;
}

[a-zA-Z_][a-zA-Z0-9_]* {
    PRINT_TOKEN("T_IDENTIFIER");
    yylval->string = new std::string(YYText(), YYLeng());
    return token::T_IDENTIFIER;
}

[-+]?[0-9]+ {
    PRINT_TOKEN("T_INTEGER_VAL");
    yylval->integer = std::stoll(std::string(YYText(), YYLeng()));
    return token::T_INTEGER_VAL;
}

[-+]?[0-9]+\.[0-9]* {
    PRINT_TOKEN("T_RATIONAL_VAL");
    yylval->rational = std::stod(std::string(YYText(), YYLeng()));
    return token::T_RATIONAL_VAL;
}

. {
    std::cerr << "Unexpected '" << YYText() << "'" << std::endl;
    std::cerr << "Ln " << yyloc->begin.line << ", Col " << yyloc->begin.column << std::endl;
    exit(1);
}
