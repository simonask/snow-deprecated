%{
#include "y.tab.h"
#include <stdlib.h>
#include <string.h>

void yyerror(const char*);
%}

%%
[0-9]+                          { yylval = atoi(yytext); return tINTEGER; }
[0-9]+\.[0-9]+		        	{ yylval = atoi(yytext); return tFLOAT; }
0x[0-9A-Fa-f]+                  { yylval = strtol(yytext,NULL,16); return tINTEGER; }
0b[01]+                         { 
                                    int l = strlen(yytext) - 2;
                                    char dst[l]; 
                                    sprintf(dst, "%.*s", l, yytext + 2);
                                    yylval = strtol(dst,NULL,2);
                          
                                    return tINTEGER;
                                }

\'(.*)\'                        { return tSTRING; }
\"(.*)\"                        { return tSTRING; } // ' <-- This keeps TM from bugging out syntax highlighting.


if                              { return tIF; }
unless			        		{ return tUNLESS; }
elsif                           { return tELSIF; }
else                            { return tELSE; }
do                              { return tDO; }
while                           { return tWHILE; }
end                             { return tEND; }
break                           { return tBREAK; }
continue                        { return tCONTINUE; }
try                             { return tTRY; }
catch                           { return tCATCH; }
throw                           { return tTHROW; }
finally                         { return tFINALLY; }
return                          { return tRETURN; }
true			        		{ return tTRUE; }
false			        		{ return tFALSE; }
nil						        { return tNIL; }

[_#@a-zA-Z][_#@a-zA-Z0-9]+      { return tIDENTIFIER; }

≥|>=                            { return tGTE; }
≤|<=                            { return tLTE; }
\*\*                            { return tPOW; }
[\n;]                           { return tLSEP; }
\/\/.+                          { /* Naïve but otherwise good solution - eat short comments. */; }
[ \t]                           { /* Eat whitespaces */; }
.                               { return (int) yytext[0]; }
%%

int yywrap(void) {
    return 1;
}