%{
#include "y.tab.h"
#include <stdlib.h>
#include <string.h>

void yyerror(const char*);
%}

%%
[0-9]+                  { yylval = atoi(yytext); return tINTEGER; }
[0-9]+\.[0-9]+			{ yylval = atoi(yytext); return tFLOAT; }
0x[0-9A-Fa-f]+          { yylval = strtol(yytext,NULL,16); return tINTEGER; }
if                      { return tIF; }
unless					{ return tUNLESS; }
do                      { return tDO; }
while                   { return tWHILE; }
end                     { return tEND; }
break                   { return tBREAK; }
continue                { return tCONTINUE; }
true					{ return tTRUE; }
false					{ return tFALSE; }
nil						{ return tNIL; }
[a-zA-Z_]+              { return tIDENTIFIER; }
≥|>=                    { return tGTE; }
≤|<=                    { return tLTE; }
\*\*                    { return tPOW; }
[\n;]                   { return tLSEP; }
[ \t]                   { /* Eat whitespaces */; }
.                       { return (int) yytext[0]; }
%%

int yywrap(void) {
    return 1;
}