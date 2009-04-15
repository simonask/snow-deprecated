%{
#include <string>
#include <iostream>
#include <list>
#include "ASTNode.h"
#include "lib/Runtime.h"

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
    // General purpose types
    ast::Node* node;
    std::list<ast::Node*>* list;

    // More specific AST-types
    ast::Identifier* identifier;
    ast::Literal* literal;
    ast::FunctionDefinition* function_defintion;
    ast::Sequence* sequence;
}

%parse-param { Driver& driver }

%token END_FILE 0

%token <node> END RETURN BREAK CONTINUE THROW CATCH TRY FINALLY SELF IT
%token <literal> INTEGER FLOAT STRING TRUE FALSE NIL
%token <identifier> IDENTIFIER 

%left <node> '.' '[' ']' '{' '}' '(' ')'
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

%type <node> statement conditional function command return_cmd expression
             function_call assignment mathematical_operation logical_operation
             bitwise_operation scoped_var local_var variable else_cond

%type <literal> literal symbol
%type <function_defintion> program closure scope
%type <sequence> sequence arguments arg_list
%type <list> parameters //elsif_cond

%expect 76

%{

#include "Driver.h"
#include "Scanner.h"

#undef yylex
#define yylex driver.lexer->lex

%}

%%

program:    sequence                                        { $$ = new ast::FunctionDefinition; $$->sequence = $1; driver.scope = $$; }
            ;

statement:  function                                        { $$ = $1; }
            | conditional                                   { $$ = $1; }
            | WHILE expression EOL sequence END             { $$ = new ast::Loop($2, $4); }
            | function WHILE expression                     { $$ = new ast::Loop($1, $3); }
            | DO sequence WHILE expression                  { $$ = new ast::Loop($2, $4); }
//          | TRY sequence catch_sqnc finally_stmt END
            ;

conditional:  IF expression EOL sequence else_cond END      { 
                                                              if ($5 != NULL) {
                                                                  $$ = new ast::IfElseCondition($2, $4, $5);
                                                              } else {
                                                                  $$ = new ast::IfCondition($2, $4);
                                                              }
                                                            }
            | UNLESS expression EOL sequence else_cond END  {
                                                              if ($5 != NULL) {
                                                                  $$ = new ast::IfElseCondition($2, $4, $5, true);
                                                              } else {
                                                                  $$ = new ast::IfCondition($2, $4, true);
                                                              }
                                                            }
            | function IF expression                        { $$ = new ast::IfCondition($3, $1); }
            | function UNLESS expression                    { $$ = new ast::IfCondition($3, $1, true); }
            ;

//elsif_cond: /* Nothing */                                   { $$ = new std::list<ast::Node*>; }
//            | elsif_cond ELSIF expression EOL sequence      { $$ = $1; $1->push_back(new ast::IfCondition($3, $5)); }
//            ;

else_cond:  /* Nothing */                                   { $$ = NULL; }
            | ELSE EOL sequence                             { $$ = $3; }
            ;

sequence:   /* Nothing */                                   { $$ = new ast::Sequence; }
            | sequence EOL                                  { $$ = $1; }
            | sequence statement                            { $$ = $1; $1->add($2); }
            ;

function:   expression                                      { $$ = $1; }
            | command                                       { $$ = $1; }
            ;

command:    return_cmd                                      { $$ = $1; }
//          | throw_cmd                                     { $$ = $1; }
            | BREAK                                         { $$ = $1; }
            | CONTINUE                                      { $$ = $1; }
            ;
/*
throw_cmd:  THROW variable                                  { $$ = new ast::Throw($1); }
            ;

catch_stmt: CATCH variable
            | CATCH variable IF expression
            | CATCH variable UNLESS expression
            ;

catch_sqnc: // Nothing
            | catch_sqnc catch_stmt EOL sequence
            ;

finally_stmt: // Nothing
            | FINALLY sequence
            ;
*/
return_cmd: RETURN                                          { $$ = new ast::Return; }
            | RETURN expression                             { $$ = new ast::Return($2); }
            ;
            
scoped_var: '.' IDENTIFIER                                  { $$ = new ast::Get(new ast::Self, $2); }
            | IDENTIFIER '.' IDENTIFIER                     { $$ = new ast::Get($1, $3); }
            | scoped_var '.' IDENTIFIER                     { $$ = new ast::Get($1, $3); }
            ;

local_var:  IDENTIFIER                                      { $$ = $1; }
            ;
            
variable:   scoped_var                                      { $$ = $1; }
            | local_var                                     { $$ = $1; }
            | SELF                                          { $$ = $1; }
            | IT                                            { $$ = $1; }
            ;

parameters: IDENTIFIER                                      { $$ = new std::list<ast::Node*>; $$->push_back($1); }
            | parameters ',' IDENTIFIER                     { $1->push_back($3); }
            ;

arg_list:   expression                                      { $$ = new ast::Sequence($1); }
            | arg_list ',' expression                       { $$ = $1; $$->add($3); }
            ;

closure:    '[' parameters ']' scope                        { $$ = $4;
                                                              for (auto iter = $2->begin(); iter != $2->end(); iter++)
                                                                  $4->add_argument(static_cast<ast::Identifier*>(*iter));
                                                            }
            | scope                                         { $$ = $1; }
            ;

scope:      '{' sequence '}'                                { $$ = new ast::FunctionDefinition; $$->sequence = $2; }
            ;

symbol:     '#' IDENTIFIER                                  { $$ = new ast::Literal(value_to_string($2->name), ast::Literal::SYMBOL_TYPE); }
            | '#' STRING                                    { $$ = new ast::Literal($2->string, ast::Literal::SYMBOL_TYPE); }
            ;

literal:    INTEGER                                         { $$ = $1; }
            | FLOAT                                         { $$ = $1; }
            | STRING                                        { $$ = $1; }
            | TRUE                                          { $$ = $1; }
            | FALSE                                         { $$ = $1; }
            | NIL                                           { $$ = $1; }
            | symbol                                        { $$ = $1; }
            ;

arguments:  '(' ')'                                         { $$ = new ast::Sequence; }
            | '(' arg_list ')'                              { $$ = $2; }
            ;

function_call: scoped_var arguments closure                 { $2->add($3); $$ = new ast::Call(dynamic_cast<ast::Get*>($1)->self, dynamic_cast<ast::Get*>($1)->member, $2); }
            | local_var arguments closure                   { $2->add($3); $$ = new ast::Call($1, NULL, $2); }
            | expression '.' IDENTIFIER arguments closure   { $4->add($5); $$ = new ast::Call($1, $3, $4); }
            | scoped_var arguments                          { $$ = new ast::Call(dynamic_cast<ast::Get*>($1)->self, dynamic_cast<ast::Get*>($1)->member, $2); }
            | local_var arguments                           { $$ = new ast::Call($1, NULL, $2); }
            | expression '.' IDENTIFIER arguments           { $$ = new ast::Call($1, $3, $4); }
            | scoped_var closure                            { $$ = new ast::Call(dynamic_cast<ast::Get*>($1)->self, dynamic_cast<ast::Get*>($1)->member, new ast::Sequence($2)); }
            | local_var closure                             { $$ = new ast::Call($1, NULL, new ast::Sequence($2)); }
            | expression '.' IDENTIFIER closure             { $$ = new ast::Call($1, $3, new ast::Sequence($4)); }
            ;

assignment: local_var ':' expression                        { $$ = new ast::Assignment(dynamic_cast<ast::Identifier*>($1), $3); }
            | scoped_var ':' expression                     { $$ = new ast::Set(dynamic_cast<ast::Get*>($1), $3); }
            ;

mathematical_operation: expression '+' expression           { RefPtr<ast::Sequence> args = new ast::Sequence($3);
                                                              $$ = new ast::Call($1, new ast::Identifier("+"), args); }
            | expression '-' expression                     { RefPtr<ast::Sequence> args = new ast::Sequence($3);
                                                              $$ = new ast::Call($1, new ast::Identifier("-"), args); }
            | expression '*' expression                     { RefPtr<ast::Sequence> args = new ast::Sequence($3);
                                                              $$ = new ast::Call($1, new ast::Identifier("*"), args); }
            | expression '/' expression                     { RefPtr<ast::Sequence> args = new ast::Sequence($3);
                                                              $$ = new ast::Call($1, new ast::Identifier("/"), args); }
            | '-' expression %prec NEG                      { $$ = new ast::Call($2, new ast::Identifier("-")); }
            | expression '%' expression                     { RefPtr<ast::Sequence> args = new ast::Sequence($3);
                                                              $$ = new ast::Call($1, new ast::Identifier("%"), args); }
            | expression POW expression                     { RefPtr<ast::Sequence> args = new ast::Sequence($3);
                                                              $$ = new ast::Call($1, new ast::Identifier("**"), args); }
            ;

logical_operation: expression '=' expression                { RefPtr<ast::Sequence> args = new ast::Sequence($3);
                                                              $$ = new ast::Call($1, new ast::Identifier("="), args); }
            | expression '>' expression                     { RefPtr<ast::Sequence> args = new ast::Sequence($3);
                                                              $$ = new ast::Call($1, new ast::Identifier(">"), args); }
            | expression GTE expression                     { RefPtr<ast::Sequence> args = new ast::Sequence($3);
                                                              $$ = new ast::Call($1, new ast::Identifier(">="), args); }
            | expression '<' expression                     { RefPtr<ast::Sequence> args = new ast::Sequence($3);
                                                              $$ = new ast::Call($1, new ast::Identifier("<"), args); }
            | expression LTE expression                     { RefPtr<ast::Sequence> args = new ast::Sequence($3);
                                                              $$ = new ast::Call($1, new ast::Identifier("<="), args); }
            | expression LOG_AND expression                 { RefPtr<ast::Sequence> args = new ast::Sequence($3);
                                                              $$ = new ast::Call($1, new ast::Identifier("and"), args); }
            | expression LOG_OR expression                  { RefPtr<ast::Sequence> args = new ast::Sequence($3);
                                                              $$ = new ast::Call($1, new ast::Identifier("or"), args); }
            | LOG_NOT expression                            { $$ = new ast::Call($2, new ast::Identifier("not")); }
            ;

bitwise_operation: expression LSHFT expression              { RefPtr<ast::Sequence> args = new ast::Sequence($3);
                                                              $$ = new ast::Call($1, new ast::Identifier("<<"), args); }
            | expression RSHFT expression                   { RefPtr<ast::Sequence> args = new ast::Sequence($3);
                                                              $$ = new ast::Call($1, new ast::Identifier(">>"), args); }
            | expression '|' expression                     { RefPtr<ast::Sequence> args = new ast::Sequence($3);
                                                              $$ = new ast::Call($1, new ast::Identifier("|"), args); }
            | expression '&' expression                     { RefPtr<ast::Sequence> args = new ast::Sequence($3);
                                                              $$ = new ast::Call($1, new ast::Identifier("&"), args); }
            | expression '^' expression                     { RefPtr<ast::Sequence> args = new ast::Sequence($3);
                                                              $$ = new ast::Call($1, new ast::Identifier("^"), args); }
            | '~' expression                                { $$ = new ast::Call($1, new ast::Identifier("~")); }
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
