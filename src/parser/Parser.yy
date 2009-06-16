%{
#include <string>
#include <iostream>
#include <list>
#include "codegen/ASTNode.h"
#include "runtime/Runtime.h"

// Forward declaration of the Driver class.
namespace snow { class Driver; }
%}

%require "2.3"
%start program
%defines
%name-prefix="snow"
%skeleton "lalr1.cc"
%define "parser_class_name" "Parser"
/* 
  Replace the directives above with the following when we switch to 
  bison 2.4.1:

%code requires {
#include <string>
#include <iostream>
#include <list>
#include "codegen/ASTNode.h"
#include "runtime/Runtime.h"

// Forward declaration of the Driver class.
namespace snow { class Driver; }
}

//%require "2.3"
%start program
%defines
//%name-prefix="snow"
%define namespace "snow"
%skeleton "lalr1.cc"
%define "parser_class_name" "Parser"

%locations
%initial-action
{
    @$.begin.filename = @$.end.filename = &driver.streamname;
};

%union {
    // General purpose types
    ast::Node* node;
    ast::Member* member;
    std::list<RefPtr<ast::Identifier>>* identifier_list;
    std::list<RefPtr<ast::IfCondition>>* condition_list;

    // More specific AST-types
    ast::Identifier* identifier;
    ast::Literal* literal;
    ast::FunctionDefinition* function_definition;
    ast::Sequence* sequence;
}

%parse-param { Driver& driver }

%token END_FILE 0

%left <node> END RETURN BREAK CONTINUE THROW CATCH TRY FINALLY SELF IT
%left <literal> INTEGER FLOAT STRING TRUE FALSE NIL

%left <identifier> IDENTIFIER OPERATOR_LRA
%left <identifier> OPERATOR_FOURTH
%left <identifier> OPERATOR_THIRD
%left <identifier> OPERATOR_SECOND
%left <identifier> OPERATOR_FIRST

%token <node> INTERPOLATION
%token '.' ',' '[' ']' '{' '}' '(' ')' ':' '#'
%token EOL DO UNLESS ELSE IF ELSEIF WHILE

%type <node> statement conditional function command return_cmd expression
             function_call assignment operation variable
             else_cond string_data string_literal literal
%type <member> scoped_var
%type <identifier> local_var
%type <literal> symbol
%type <function_definition> program closure scope
%type <sequence> sequence arguments arg_list
%type <identifier_list> parameters
%type <condition_list> elsif_cond

%expect 82

%{

#include "Driver.h"
#include "Scanner.h"

#undef yylex
#define yylex driver.lexer->lex

%}

%%

program:    sequence                                        { $$ = new ast::FunctionDefinition; $$->sequence = $1; $$->file = strdup(driver.streamname.c_str()); driver.scope = $$; }
            ;

statement:  function                                        { $$ = $1; }
            | conditional                                   { $$ = $1; }
            | WHILE expression EOL sequence END             { $$ = new ast::Loop($2, $4); }
            | function WHILE expression                     { $$ = new ast::Loop($1, $3); }
            | DO sequence WHILE expression                  { $$ = new ast::Loop($2, $4); }
            ;

conditional:  IF expression EOL sequence elsif_cond else_cond END     { 
                                                                        if ($6 != NULL) {
                                                                            if ($5->empty()) {
                                                                                $$ = new ast::IfElseCondition($2, $4, $6);
                                                                            } else {
                                                                                $$ = new ast::IfElseIfElseCondition($2, $4, $6);
                                                                                for (auto iter = $5->begin(); iter != $5->end(); iter++)
                                                                                    dynamic_cast<ast::IfElseIfElseCondition*>($$)->else_if.push_back(*iter);
                                                                            }
                                                                        } else {
                                                                            if ($5->empty()) {
                                                                                $$ = new ast::IfCondition($2, $4);
                                                                            } else {
                                                                                $$ = new ast::IfElseIfElseCondition($2, $4, $6);
                                                                                for (auto iter = $5->begin(); iter != $5->end(); iter++)
                                                                                    dynamic_cast<ast::IfElseIfElseCondition*>($$)->else_if.push_back(*iter);
                                                                            }
                                                                        }
                                                                      }
            | UNLESS expression EOL sequence elsif_cond else_cond END {
                                                                        if ($6 != NULL) {
                                                                            if ($5->empty()) {
                                                                                $$ = new ast::IfElseCondition($2, $4, $6, true);
                                                                            } else {
                                                                                $$ = new ast::IfElseIfElseCondition($2, $4, $6, true);
                                                                                for (auto iter = $5->begin(); iter != $5->end(); iter++)
                                                                                    dynamic_cast<ast::IfElseIfElseCondition*>($$)->else_if.push_back(*iter);
                                                                            }
                                                                        } else {
                                                                            if ($5->empty()) {
                                                                                $$ = new ast::IfCondition($2, $4, true);
                                                                            } else {
                                                                                $$ = new ast::IfElseIfElseCondition($2, $4, $6, true);
                                                                                for (auto iter = $5->begin(); iter != $5->end(); iter++)
                                                                                    dynamic_cast<ast::IfElseIfElseCondition*>($$)->else_if.push_back(*iter);
                                                                            }
                                                                        }
                                                                      }

            | function IF expression                        { $$ = new ast::IfCondition($3, $1); }
            | function UNLESS expression                    { $$ = new ast::IfCondition($3, $1, true); }
            ;

elsif_cond: /* Nothing */                                   { $$ = new std::list<RefPtr<ast::IfCondition>>; }
            | elsif_cond ELSEIF expression EOL sequence     { $$ = $1; $1->push_back(new ast::IfCondition($3, $5)); }
            ;

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
            | BREAK                                         { $$ = $1; }
            | CONTINUE                                      { $$ = $1; }
            ;

return_cmd: RETURN                                          { $$ = new ast::Return; }
            | RETURN expression                             { $$ = new ast::Return($2); }
            ;
            
scoped_var: '.' IDENTIFIER                                  { $$ = new ast::Member(new ast::Self, $2); }
            | IDENTIFIER '.' IDENTIFIER                     { $$ = new ast::Member($1, $3); }
            | scoped_var '.' IDENTIFIER                     { $$ = new ast::Member($1, $3); }
            | expression '.' IDENTIFIER                     { $$ = new ast::Member($1, $3); }
            ;

local_var:  IDENTIFIER                                      { $$ = $1; }
            ;
            
variable:   scoped_var                                      { $$ = $1; }
            | local_var                                     { $$ = $1; }
            | SELF                                          { $$ = $1; }
            | IT                                            { $$ = $1; }
            ;

parameters: IDENTIFIER                                      { $$ = new std::list<RefPtr<ast::Identifier>>; $$->push_back($1); }
            | parameters ',' IDENTIFIER                     { $1->push_back($3); }
            ;

arg_list:   expression                                      { $$ = new ast::Sequence($1); }
            | arg_list ',' expression                       { $$ = $1; $$->add($3); }
            ;

closure:    '[' parameters ']' scope                        { $$ = $4;
                                                              for (auto iter = $2->begin(); iter != $2->end(); iter++)
                                                                  $4->add_argument(*iter);
                                                            }
            | scope                                         { $$ = $1; }
            ;

scope:      '{' sequence '}'                                { $$ = new ast::FunctionDefinition; $$->sequence = $2; $$->file = strdup(driver.streamname.c_str()); $$->line = yylloc.begin.line; }
            ;

symbol:     '#' IDENTIFIER                                  { $$ = new ast::Literal(value_to_string($2->name), ast::Literal::SYMBOL_TYPE); }
            | '#' STRING                                    { $$ = new ast::Literal($2->string, ast::Literal::SYMBOL_TYPE); }
            ;

literal:    INTEGER                                         { $$ = $1; }
            | FLOAT                                         { $$ = $1; }
            | TRUE                                          { $$ = $1; }
            | FALSE                                         { $$ = $1; }
            | NIL                                           { $$ = $1; }
            | string_literal                                { $$ = $1; }
            | symbol                                        { $$ = $1; }
            ;

string_data: STRING                                         { $$ = $1; }
            | INTERPOLATION                                 { $$ = $1; }
            ;

string_literal: string_data                                 { $$ = $1; }
            | string_literal string_data                    { $$ = new ast::MemberCall($1, new ast::Identifier("+"), new ast::Sequence($2)); }
            ;

arguments:  '(' ')'                                         { $$ = new ast::Sequence; }
            | '(' arg_list ')'                              { $$ = $2; }
            ;

function_call: scoped_var arguments closure                 { $2->add($3); $$ = new ast::MemberCall($1->object, $1->member, $2); }
            | local_var arguments closure                   { $2->add($3); $$ = new ast::ExpressionCall($1, $2); }
            | expression '.' IDENTIFIER arguments closure   { $4->add($5); $$ = new ast::MemberCall($1, $3, $4); }
            | scoped_var arguments                          { $$ = new ast::MemberCall($1->object, $1->member, $2); }
            | local_var arguments                           { $$ = new ast::ExpressionCall($1, $2); }
            | expression '.' IDENTIFIER arguments           { $$ = new ast::MemberCall($1, $3, $4); }
            | scoped_var closure                            { $$ = new ast::MemberCall($1->object, $1->member, new ast::Sequence($2)); }
            | local_var closure                             { $$ = new ast::ExpressionCall($1, new ast::Sequence($2)); }
            | expression '.' IDENTIFIER closure             { $$ = new ast::MemberCall($1, $3, new ast::Sequence($4)); }
            ;

assignment: local_var ':' expression                        { $$ = new ast::LocalAssignment($1, $3); }
            | scoped_var ':' expression                     { $$ = new ast::MemberAssignment($1->object, $1->member, $3); }
            ;

operation:  OPERATOR expression                             { $$ = new ast::MemberCall($2, $1, new ast::Sequence); }
            | expression OPERATOR expression                { $$ = new ast::MemberCall($1, $2, new ast::Sequence($3)); }
            ;

expression: literal                                         { $$ = $1; }
            | closure                                       { $$ = $1; }
            | variable                                      { $$ = $1; }
            | function_call                                 { $$ = $1; }
            | assignment                                    { $$ = $1; }
            | operation                                     { $$ = $1; }
            | '(' expression ')'                            { $$ = $2; }
            ;

%%

void snow::Parser::error(const Parser::location_type& l, const std::string& m)
{
    driver.error(l, m);
}
