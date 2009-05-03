%{
#include <string>
#include <sstream>
#include "Scanner.h"
#include "ASTNode.h"
#include "Driver.h"

typedef snow::Parser::token token;
typedef snow::Parser::token_type token_type;

#define yyterminate() return token::END_FILE
#define YY_NO_UNISTD_H

std::stringstream string_buffer, interpolation_buffer;
%}

%option c++
%option prefix="Snow"
%option batch
%option yywrap nounput 
%option stack
%x snow_string_single
%x snow_string_double
%x interpolation
%x snow_comment_long

%{
#define YY_USER_ACTION  yylloc->columns(yyleng);
%}

%% 

%{
	yylloc->step();
%}

\"                                     { BEGIN(snow_string_double); string_buffer.str(""); } /* " */
<snow_string_double>\"                 { BEGIN(INITIAL); yylval->literal = new ast::Literal(string_buffer.str(), ast::Literal::STRING_TYPE); return token::STRING; } /* " */
<snow_string_double>\\n                { string_buffer << "\n"; }
<snow_string_double>\\t                { string_buffer << "\t"; }
<snow_string_double>\\r                { string_buffer << "\r"; }
<snow_string_double>\\b                { string_buffer << "\b"; }
<snow_string_double>\\f                { string_buffer << "\f"; }
<snow_string_double>\\(.|\n)           { string_buffer << yytext[1]; }
<snow_string_double>\$\(               { BEGIN(interpolation); yylval->literal = new ast::Literal(string_buffer.str(), ast::Literal::STRING_TYPE); string_buffer.str(""); return token::STRING; }
<snow_string_double>[^\$\(\\\n\"]+     { string_buffer << yytext; } /* " */
<snow_string_double>.                  { string_buffer << yytext; }

<interpolation>\)                      { BEGIN(snow_string_double); yylval->node = new ast::Call(Driver::parse(interpolation_buffer.str())->sequence, new ast::Identifier("to_string")); interpolation_buffer.str(""); string_buffer.str(""); return token::INTERPOLATION; }
<interpolation>[^\)]+                  { interpolation_buffer << yytext; }

\'                                     { BEGIN(snow_string_single); string_buffer.str(""); } /* ' */
<snow_string_single>\'                 { BEGIN(INITIAL); yylval->literal = new ast::Literal(string_buffer.str(), ast::Literal::STRING_TYPE); return token::STRING; } /* ' */
<snow_string_single>\\n                { string_buffer << "\n"; }
<snow_string_single>\\t                { string_buffer << "\t"; }
<snow_string_single>\\r                { string_buffer << "\r"; }
<snow_string_single>\\b                { string_buffer << "\b"; }
<snow_string_single>\\f                { string_buffer << "\f"; }
<snow_string_single>\\(.|\n)           { string_buffer << yytext[1]; }
<snow_string_single>[^\\\n\']+         { string_buffer << yytext; } /* ' */

"//".+                                 { /* Do absolutely nothing. */ }
                                       
"/*"                                   { BEGIN(snow_comment_long); }
<snow_comment_long>"*/"                { BEGIN(INITIAL); }
<snow_comment_long>[^\n\/\*]+          { /* Do absolutely nothing. */ }
<snow_comment_long>\n                  { yylloc->lines(yyleng); yylloc->step(); }
<snow_comment_long>.                   { /* Do absolutely nothing. */ }
                                       
[0-9]+                                 { yylval->literal = new ast::Literal(yytext, ast::Literal::INTEGER_DEC_TYPE); return token::INTEGER; }
0b[01]+                                { yylval->literal = new ast::Literal(std::string(yytext).substr(2, std::string::npos), ast::Literal::INTEGER_BIN_TYPE); return token::INTEGER; }
0x[0-9a-fA-F]+                         { yylval->literal = new ast::Literal(std::string(yytext).substr(2, std::string::npos), ast::Literal::INTEGER_HEX_TYPE); return token::INTEGER; }
[0-9]+\.[0-9]+                         { yylval->literal = new ast::Literal(yytext, ast::Literal::FLOAT_TYPE); return token::FLOAT; }
                                       
self                                   { yylval->node = new ast::Self; return token::SELF; }
it                                     { yylval->node = new ast::It; return token::IT; }
if                                     { return token::IF; }
unless                                 { return token::UNLESS; }
else                                   { return token::ELSE; }
"else if"                              { return token::ELSEIF; }
do                                     { return token::DO; }
while                                  { return token::WHILE; }
end                                    { return token::END; }
break                                  { yylval->node = new ast::Break(); return token::BREAK; }
continue                               { yylval->node = new ast::Continue(); return token::CONTINUE; }
return                                 { return token::RETURN; }
true                                   { yylval->literal = new ast::Literal(ast::Literal::TRUE_TYPE); return token::TRUE; }
false                                  { yylval->literal = new ast::Literal(ast::Literal::FALSE_TYPE); return token::FALSE; }
nil                                    { yylval->literal = new ast::Literal(ast::Literal::NIL_TYPE); return token::NIL; }
[_$@a-zA-Z][_$@a-zA-Z0-9]*             { yylval->identifier = new ast::Identifier(yytext); return token::IDENTIFIER; }
;                                      { return token::EOL; }
\n                                     { yylloc->lines(yyleng); yylloc->step(); return token::EOL; }
[ \t\r]                                { yylloc->step(); /* Eat whitespaces */ }
[.,\[\]{}():#]                         { return static_cast<token_type>(*yytext); }
[^ \t\r\n.,\[\]{}():#a-zA-Z0-9\"]+     { yylval->identifier = new ast::Identifier(yytext); return token::OPERATOR; } /* " */

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
