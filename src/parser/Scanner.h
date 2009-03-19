#ifndef SNOW_SCANNER_H
#define SNOW_SCANNER_H

#ifndef YY_DECL
#define	YY_DECL	snow::Parser::token_type snow::Scanner::lex(snow::Parser::semantic_type* yylval, snow::Parser::location_type* yylloc)
#endif

#ifndef __FLEX_LEXER_H
#define yyFlexLexer SnowFlexLexer
#include "FlexLexer.h"
#undef yyFlexLexer
#endif

#include "Parser.h"

namespace snow {

    class Scanner : public SnowFlexLexer
    {
    public:
        Scanner(std::istream* arg_yyin = 0, std::ostream* arg_yyout = 0);
        virtual ~Scanner();
        virtual Parser::token_type lex(Parser::semantic_type* yylval, Parser::location_type* yylloc);
    };

}

#endif
