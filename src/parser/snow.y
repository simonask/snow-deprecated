%{
#include <iostream>
#include <cmath>

extern int yydebug;

using namespace std; 

int yylex(void);
void yyerror(const char*);
%}

%token tINTEGER tFLOAT tTRUE tFALSE tNIL tIDENTIFIER tEND tBREAK tCONTINUE
%left tDO tWHILE tIF tUNLESS tLSEP
%left '='
%left '>' '<' tGTE tLTE
%left '+' '-'
%left '*' '/' '%'
%left '&' /* unary ampersand */
%left NEG /* unary minus */
%right tPOW

%%
program:    sequence									{ cout << $1 << endl; }
            ;

statement:  function                                  	{ $$ = $1; }
            | tIF expression tLSEP sequence tEND        { $$ = $4; }
			| tUNLESS expression tLSEP sequence tEND    { $$ = $4; }
            | function tIF expression                   { $$ = $1; }
            | function tUNLESS expression               { $$ = $1; }
            | tWHILE expression tLSEP sequence tEND     { $$ = $4; }
            | function tWHILE expression                { $$ = $1; }
            | tDO statement tWHILE expression           { $$ = $2; }
            ;

sequence:   /* Nothing */								{ cout << "Nothing" << endl; }
			| tLSEP										{ cout << "Separator" << endl; }
			| sequence statement					    { $$ = $2; cout << "Sequence - Statement goes " << $2 << endl; }
			| sequence tLSEP							{ $$ = $1; cout << "Sequence - tLSEP goes " << $1 << endl; }
            ;

function:   expression                                  { $$ = $1; }
            | command                                   { $$ = $1; }
            ;

command:    tBREAK                                      { $$ = 0; }
            | tCONTINUE                                 { $$ = 0; }
            ;

variable:   tIDENTIFIER                                 { $$ = 0; }
            | '.' tIDENTIFIER                           { $$ = 0; }
            | variable '.' tIDENTIFIER                  { $$ = 0; }
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

literal:	tINTEGER                                    { $$ = $1; cout << "INTEGER" << endl;}
			| tFLOAT									{ $$ = $1; cout << "FLOAT" << endl;}
			| tTRUE										{ $$ = $1; cout << "TRUE" << endl; }
			| tFALSE									{ $$ = $1; cout << "FALSE" << endl;}
			| tNIL										{ $$ = $1; cout << "NIL" << endl;}

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