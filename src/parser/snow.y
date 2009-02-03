%{
#include <iostream>
#include <cmath>

extern int yydebug;

using namespace std; 

int yylex(void);
void yyerror(const char*);
%}

%token tINTEGER tFLOAT tTRUE tFALSE tNIL tIDENTIFIER tEND tRETURN tBREAK tCONTINUE tTHROW tCATCH tTRY tFINALLY
%left tDO tWHILE tIF tELSIF tELSE tUNLESS tLSEP
%left '='
%left '>' '<' tGTE tLTE
%left '+' '-'
%left '*' '/' '%'
%left '&' /* unary ampersand */
%left NEG /* unary minus */
%right tPOW

%expect 41

%%
program:    sequence									{ cout << $1 << endl; }
            ;

statement:  function                                      	{ $$ = $1; }
            | conditional                                   { $$ = $1; }
            | tWHILE expression tLSEP sequence tEND         { $$ = $4; }
            | function tWHILE expression                    { $$ = $1; }
            | tDO sequence tWHILE expression                { $$ = $2; }
            | tTRY sequence catch_sqnc finally_stmt tEND    { $$ = 0; }
            ;
            
conditional:tIF expression tLSEP sequence elsif_cond else_cond tEND         { $$ = 0; }
            | tUNLESS expression tLSEP sequence elsif_cond else_cond tEND   { $$ = 0; }
            | function tIF expression                                       { $$ = 0; }
            | function tUNLESS expression                                   { $$ = 0; }
            ;

elsif_cond: /* Nothing */
            | elsif_cond tELSIF expression tLSEP sequence                   { $$ = 0; }
            ;

else_cond:  /* Nothing */
            | tELSE tLSEP sequence                      { $$ = 0; }
            ;

sequence:   /* Nothing */								
			| tLSEP										
			| sequence statement					    { $$ = $2; }
			| sequence tLSEP							{ $$ = $1; }
            ;

function:   expression                                  { $$ = $1; }
            | command                                   { $$ = $1; }
            ;

command:    return_cmd                                  { $$ = 0; }
            | throw_cmd                                 { $$ = 0; }
            | tBREAK                                    { $$ = 0; }
            | tCONTINUE                                 { $$ = 0; }                                    
            ;

throw_cmd:  tTHROW variable                             { $$ = 0; }
            ;

catch_stmt: tCATCH variable                             { $$ = 0; }
            | tCATCH variable tIF expression            { $$ = 0; }
            | tCATCH variable tUNLESS expression        { $$ = 0; }
            ;

catch_sqnc: /* Nothing */
            | catch_sqnc catch_stmt tLSEP sequence      { $$ = 0; }
            ;

finally_stmt: /* Nothing */
            | tFINALLY sequence                         { $$ = 0; }
            ;

return_cmd: tRETURN                                     { $$ = 0; /* Void return */ }
            | tRETURN expression                        { $$ = 0; /* Return $2 */}
            ;

scoped_var: '.' tIDENTIFIER                             { $$ = 0; }
            | scoped_var '.' tIDENTIFIER                { $$ = 0; }
            ;

local_var:  tIDENTIFIER                                 { $$ = 0; }
            ;

variable:   scoped_var                                  { $$ = $0; }
            | local_var                                 { $$ = $0; }
            ;

parameters: tIDENTIFIER                                 { $$ = $1; }
            | parameters ',' tIDENTIFIER                { $$ = $3; }
            ;

arguments:  expression
            | arguments ',' expression                  { $$ = $3; }
            ;

closure:    '[' parameters ']' scope					{ $$ = $1; }
            | scope										{ $$ = $1; }
            ;

scope: 		'{' sequence '}'							{ $$ = $2; }
			;

literal:	tINTEGER                                    { $$ = $1; }
			| tFLOAT									{ $$ = $1; }
			| tTRUE										{ $$ = $1; }
			| tFALSE									{ $$ = $1; }
			| tNIL										{ $$ = $1; }
            ;

expression: literal										{ $$ = $1; }
			| closure									{ $$ = $1; }
            | variable                                  { $$ = $1; }
            | variable '(' ')'                          { $$ = $1; }
            | variable '(' arguments ')'                { $$ = $1; }
            | variable ':' expression                   { $$ = $3; }
            | expression '+' expression                 { $$ = $1 + $3; }
            | expression '-' expression                 { $$ = $1 - $3; }
            | expression '*' expression                 { $$ = $1 * $3; }
            | expression '/' expression                 { $$ = $1 / $3; }
            | '-' expression %prec NEG                  { $$ = -$2; }
            | expression '%' expression                 { $$ = $1 % $3; }
            | expression tPOW expression                { $$ = pow($1,$3); }
            | expression '=' expression                 { $$ = $1 == $3; }
            | expression '>' expression                 { $$ = $1 > $3; }
            | expression tGTE expression                { $$ = $1 >= $3; }
            | expression '<' expression                 { $$ = $1 < $3; }
            | expression tLTE expression                { $$ = $3 <= $3; }
            | '&' expression                            { $$ = $2; }
            | '(' expression ')'                        { $$ = $2; }
            ;
%%

void yyerror(const char* s) {
    cerr << s << endl;
}

int main(void) {
    //yydebug = 1;
    yyparse(); 
    return 0;
}