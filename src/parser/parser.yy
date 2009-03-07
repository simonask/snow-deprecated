%{
#include <string>
#include <iostream>
#include "ASTNode.h"

namespace snow { class Driver; }
%}

%require "2.3"
%start program
%defines
%skeleton "lalr1.cc"
%name-prefix="snow"
%define "parser_class_name" "Parser"

%locations
%initial-action
{
    @$.begin.filename = @$.end.filename = &driver.streamname;
};

%union {
    ast::Node* node;
    ast::Identifier* identifier;
    ast::Literal* literal;
}

%parse-param { Driver& driver }

%token END_FILE 0

%token <node> END RETURN BREAK CONTINUE THROW CATCH TRY FINALLY
%token <literal> INTEGER FLOAT STRING TRUE FALSE NIL
%token <identifier> IDENTIFIER

%token <node> '.' '[' ']' '{' '}' '(' ')'
%left <node> DO WHILE IF ELSIF ELSE UNLESS EOL
%left <node> '='
%left <node> '>' '<' GTE LTE LOG_AND LOG_OR
%left <node> '|' '&' '^' '~'
%left <node> LSHFT RSHFT
%left <node> '+' '-'
%left <node> '*' '/' '%'
%left <node> LOG_NOT
%left <node> NEG /* unary minus */
%right <node> POW

%type <node> statement conditional function command throw_cmd catch_stmt
             return_cmd variables arguments closure scope expression

%type <literal> literal
%type <identifier> instance_var local_var variable

%destructor { delete $$; } INTEGER FLOAT STRING TRUE FALSE NIL literal

%expect 89

%{

#include "Driver.h"
#include "Scanner.h"

#undef yylex
#define yylex driver.lexer->lex

%}

%%

program:    sequence             							
            ;                                               
                                                            
statement:  function                                      	
            | conditional
            | WHILE expression EOL sequence END
            | function WHILE expression
            | DO sequence WHILE expression
            | TRY sequence catch_sqnc finally_stmt END
            ;                                               

conditional:  IF expression EOL sequence elsif_cond else_cond END
            | UNLESS expression EOL sequence elsif_cond else_cond END
            | function IF expression
            | function UNLESS expression
            ;

elsif_cond: /* Nothing */                                                  
            | elsif_cond ELSIF expression EOL sequence                  
            ;

else_cond:  /* Nothing */
            | ELSE EOL sequence
            ;                                               

sequence:   /* Nothing */                                   
			| sequence EOL
			| sequence statement
            ;

function:   expression
            | command
            ;

command:    return_cmd
            | throw_cmd
            | BREAK
            | CONTINUE
            ;

throw_cmd:  THROW variable
            ;                                               

catch_stmt: CATCH variable
            | CATCH variable IF expression
            | CATCH variable UNLESS expression
            ;

catch_sqnc: /* Nothing */                                   
            | catch_sqnc catch_stmt EOL sequence
            ;

finally_stmt: /* Nothing */
            | FINALLY sequence
            ;

return_cmd: RETURN
            | RETURN expression
            | RETURN variables
            ;
            
instance_var: '.' IDENTIFIER                                { $$ = $2; }
            | instance_var '.' IDENTIFIER                   { $$ = $3; }
            ;

local_var:  IDENTIFIER                                      { $$ = $1; }
            | local_var '.' IDENTIFIER                      { $$ = $3; }
            ;
            
variable:   instance_var                                    { $$ = $1; }
            | local_var                                     { $$ = $1; }
            ;
            
variables:  variable ',' variable                           { $$ = $1; }
            | variables ',' variable                        { $$ = $1; }

parameters: /* Nothing */
            | IDENTIFIER
            | parameters ',' IDENTIFIER
            ;                                               

arguments:  expression                                      
            | arguments ',' expression
            ;                                              
                                                           
closure:    '[' parameters ']' scope					   
            | scope										   
            ;                                              
                                                           
scope: 		'{' sequence '}'							   
			;                                              

literal:	INTEGER                                         { $$ = $1; }
			| FLOAT                                         { $$ = $1; }
            | STRING                                        { $$ = $1; }
			| TRUE                                          { $$ = $1; }
			| FALSE                                         { $$ = $1; }
			| NIL                                           { $$ = $1; }
            ;                                              

expression: literal                                         { $$ = $1; }
			| closure
            | variable                                      { $$ = $1; } // These are all rather good examples of
            | variable '(' ')'                              { $$ = $1; } // temporary "just stfu, bison"-actions.
            | variable '(' arguments ')'                    { $$ = $1; } // I know. Very classy.
            | expression '.' IDENTIFIER
            | expression '.' IDENTIFIER '(' ')'
            | expression '.' IDENTIFIER '(' arguments ')'
            | variable ':' expression                       { $$ = new ast::Assignment($1, $3); }
            | variables ':' expression
            | expression '+' expression
            | expression '-' expression
            | expression '*' expression
            | expression '/' expression
            | '-' expression %prec NEG
            | expression '%' expression
            | expression POW expression
            | expression '=' expression
            | expression '>' expression
            | expression GTE expression
            | expression '<' expression
            | expression LTE expression
            | expression LOG_AND expression
            | expression LOG_OR expression
            | expression LSHFT expression
            | expression RSHFT expression
            | expression '|' expression
            | expression '&' expression
            | expression '^' expression
            | '~' expression
            | LOG_NOT expression
            | '(' expression ')'                            { $$ = $2; }
            ;

%%

void snow::Parser::error(const Parser::location_type& l, const std::string& m)
{
    driver.error(l, m);
}

int main() {
    snow::Driver driver = snow::Driver();
    bool result = driver.parse_stream(std::cin);
    return result;
}