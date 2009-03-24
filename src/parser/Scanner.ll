%{
#include <string>
#include <sstream>
#include "Scanner.h"
#include "ASTNode.h"

typedef snow::Parser::token token;
typedef snow::Parser::token_type token_type;

#define yyterminate() return token::END_FILE
#define YY_NO_UNISTD_H

std::stringstream string_buffer;
%}

%option c++
%option prefix="Snow"
%option batch
%option yywrap nounput 
%option stack
%x snow_string

%{
#define YY_USER_ACTION  yylloc->columns(yyleng);
%}

%% 

%{
    yylloc->step();
%}

\"                              { BEGIN(snow_string); string_buffer.str(""); } /* " */
<snow_string>\"                 { BEGIN(INITIAL); yylval->literal = new ast::Literal(string_buffer.str(), ast::Literal::STRING_TYPE); return token::STRING; } /* " */
<snow_string>\\n                { string_buffer << "\n"; }
<snow_string>\\t                { string_buffer << "\t"; }
<snow_string>\\r                { string_buffer << "\r"; }
<snow_string>\\b                { string_buffer << "\b"; }
<snow_string>\\f                { string_buffer << "\f"; }
<snow_string>\\(.|\n)           { string_buffer << yytext[1]; }
<snow_string>[^\\\n\"]+         { string_buffer << yytext; } /* " */

[0-9]+                          { yylval->literal = new ast::Literal(yytext, ast::Literal::INTEGER_DEC_TYPE); return token::INTEGER; }
0b[01]+                         { yylval->literal = new ast::Literal(std::string(yytext).substr(2, std::string::npos), ast::Literal::INTEGER_BIN_TYPE); return token::INTEGER; }
0x[0-9a-fA-F]+                  { yylval->literal = new ast::Literal(std::string(yytext).substr(2, std::string::npos), ast::Literal::INTEGER_HEX_TYPE); return token::INTEGER; }
[0-9]+\.[0-9]+                  { yylval->literal = new ast::Literal(yytext, ast::Literal::FLOAT_TYPE); return token::FLOAT; }

self                            { yylval->node = new ast::Self; return token::SELF; }
it                              { yylval->node = new ast::It; return token::IT; }
if                              { return token::IF; }
unless                          { return token::UNLESS; }
elsif                           { return token::ELSIF; }
else                            { return token::ELSE; }
do                              { return token::DO; }
while                           { return token::WHILE; }
end                             { return token::END; }
break                           { yylval->node = new ast::Break(); return token::BREAK; }
continue                        { yylval->node = new ast::Continue(); return token::CONTINUE; }
try                             { return token::TRY; }
catch                           { return token::CATCH; }
throw                           { return token::THROW; }
finally                         { return token::FINALLY; }
return                          { return token::RETURN; }
true                            { yylval->literal = new ast::Literal(ast::Literal::TRUE_TYPE); return token::TRUE; }
false                           { yylval->literal = new ast::Literal(ast::Literal::FALSE_TYPE); return token::FALSE; }
nil                             { yylval->literal = new ast::Literal(ast::Literal::NIL_TYPE); return token::NIL; }
and|\&\&                        { return token::LOG_AND; }
or|\|\|                         { return token::LOG_OR; }
not|\!                          { return token::LOG_NOT; }
[_#@a-zA-Z][_#@a-zA-Z0-9]*      { yylval->identifier = new ast::Identifier(yytext); return token::IDENTIFIER; }
≥|>=                            { return token::GTE; }
≤|<=                            { return token::LTE; }
\*\*                            { return token::POW; }
\<\<                            { return token::LSHFT; }
\>\>                            { return token::RSHFT; }
;                               { return token::EOL; }
\n                              { yylloc->lines(yyleng); yylloc->step(); return token::EOL; }
\/\/.+                          { /* Naïve but otherwise good solution - eat short comments. */ }
[ \t\r]                         { yylloc->step(); /* Eat whitespaces */ }
.                               { return static_cast<token_type>(*yytext); }

%%

namespace snow {

    Scanner::Scanner(std::istream* in, std::ostream* out) : SnowFlexLexer(in, out) {
    }

    Scanner::~Scanner() {
    }

}

#ifdef yylex
#undef yylex
#endif

int SnowFlexLexer::yylex() {
    std::cerr << "Warning: In SnowFlexLexer::yylex()" << std::endl;
    return 0;
}

int SnowFlexLexer::yywrap() {
    return 1;
}
