%{
#include <string>
#include <iostream>
#include <list>
#include "ASTNode.h"

// Forward declaration of the Driver class.
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
    std::list<ast::Node*>* list;
    
    ast::Identifier* identifier;
    ast::Literal* literal;
    ast::FunctionDefinition* function_defintion;
    ast::Sequence* sequence;
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
             return_cmd arguments expression function_call assignment
             mathematical_operation logical_operation bitwise_operation
             instance_var local_var variable variables

%type <literal> literal
%type <function_defintion> program closure scope
%type <sequence> sequence
%type <list> parameters 

%expect 89

%{

#include "Driver.h"
#include "Scanner.h"

#undef yylex
#define yylex driver.lexer->lex

%}

%%

program:    sequence             							{ $$ = new ast::FunctionDefinition($1); }
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

sequence:   /* Nothing */                                   { $$ = new ast::Sequence; }
            | sequence EOL                                  { $$ = $1; }
            | sequence statement                            { $$ = $1; $1->add($2); }
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

return_cmd: RETURN                                          { $$ = new ast::Return(); }
            | RETURN expression                             { $$ = new ast::Return($2); }
            | RETURN variables                              { /* FIX ME */ }
            ;
            
instance_var: '.' IDENTIFIER                                { $$ = new ast::Get(new ast::Identifier("self"), $2); }
            | instance_var '.' IDENTIFIER                   { $$ = new ast::Get($1, $3); }
            ;

local_var:  IDENTIFIER                                      { $$ = $1; }
            | local_var '.' IDENTIFIER                      { $$ = new ast::Get($1, $3); }
            ;
            
variable:   instance_var                                    { $$ = $1; }
            | local_var                                     { $$ = $1; }
            ;
            
variables:  variable ',' variable                           { $$ = $1; }
            | variables ',' variable                        { $$ = $1; }

parameters: IDENTIFIER                                      { $$ = new std::list<ast::Node*>; $$->push_back($1); }
            | parameters ',' IDENTIFIER                     { $1->push_back($3); }
            ;                                               

arguments:  expression                                      
            | arguments ',' expression
            ;

closure:    '[' parameters ']' scope					    { 
                                                                $$ = $4;
                                                                for (auto iter = $2->begin(); iter != $2->end(); iter++)
                                                                    $4->add_argument(static_cast<ast::Identifier*>(*iter));
                                                            }
            | scope										    { $$ = $1; }
            ;                                              
                                                           
scope: 		'{' sequence '}'    							{ $$ = new ast::FunctionDefinition($2); }
			;                                              

literal:	INTEGER                                         { $$ = $1; }
			| FLOAT                                         { $$ = $1; }
            | STRING                                        { $$ = $1; }
			| TRUE                                          { $$ = $1; }
			| FALSE                                         { $$ = $1; }
			| NIL                                           { $$ = $1; }
            ;                                              

function_call: variable '(' ')'                             { $$ = $1; } // Temporary.
            | variable '(' arguments ')'                    { $$ = $1; } // Likewise.
            | expression '.' IDENTIFIER
            | expression '.' IDENTIFIER '(' ')'
            | expression '.' IDENTIFIER '(' arguments ')'
            ;

assignment: variable ':' expression                         { $$ = new ast::Assignment(static_cast<ast::Identifier*>($1), $3); } // This really won't work...
            | variables ':' expression                      { /* FIX ME! */}
            ;

mathematical_operation: expression '+' expression
            | expression '-' expression
            | expression '*' expression
            | expression '/' expression
            | '-' expression %prec NEG
            | expression '%' expression
            | expression POW expression
            ;

logical_operation: expression '=' expression
            | expression '>' expression
            | expression GTE expression
            | expression '<' expression
            | expression LTE expression
            | expression LOG_AND expression
            | expression LOG_OR expression
            | LOG_NOT expression
            ;

bitwise_operation: expression LSHFT expression
            | expression RSHFT expression
            | expression '|' expression
            | expression '&' expression
            | expression '^' expression
            | '~' expression
            ;

expression: literal                                         { $$ = $1; }
            | closure                                       { $$ = $1; }
            | variable                                      { $$ = $1; }
            | function_call                                 { $$ = $1; }
            | assignment                                    { $$ = $1; }
            | mathematical_operation                        { $$ = $1; }
            | logical_operation                             { $$ = $1; }
            | bitwise_operation                             { $$ = $1; }
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