%{
#include <string>
#include <iostream>
#include "node.h"
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

%parse-param { class Driver& driver }

%union {
    Node*	node;
}

%token END_FILE 0
%token <node> INTEGER FLOAT STRING TRUE FALSE NIL IDENTIFIER END RETURN
              BREAK CONTINUE THROW CATCH TRY FINALLY
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

%type <node> program statement conditional elsif_cond else_cond sequence
             function command throw_cmd catch_stmt catch_sqnc finally_stmt
             return_cmd instance_var local_var variable variables parameters
             arguments closure scope literal expression

%expect 89

%{

#include "driver.h"
#include "scanner.h"

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
            
instance_var: '.' IDENTIFIER
            | instance_var '.' IDENTIFIER
            ;

local_var:  IDENTIFIER
            | local_var '.' IDENTIFIER
            ;
            
variable:   instance_var
            | local_var
            ;
            
variables:  variable ',' variable                           
            | variables ',' variable                        

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

literal:	INTEGER
			| FLOAT
            | STRING
			| TRUE
			| FALSE
			| NIL
            ;                                              

expression: literal
			| closure
            | variable
            | variable '(' ')'
            | variable '(' arguments ')'
            | expression '.' IDENTIFIER
            | expression '.' IDENTIFIER '(' ')'
            | expression '.' IDENTIFIER '(' arguments ')'
            | variable ':' expression
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
            | '(' expression ')'
            ;

%%

void snow::Parser::error(const Parser::location_type& l, const std::string& m)
{
    driver.error(l, m);
}

int main() {
    snow::Driver driver = snow::Driver();
    bool result = driver.parse_stream(std::cin);
    
    std::string dispres;
    if (result) { dispres = "No errors."; } else { dispres = "Error!"; }
    std::cout << "Result: " << dispres << std::endl;
}