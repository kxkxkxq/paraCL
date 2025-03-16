//-------------------------------------------------------------------------------------------------
//
//  Grammar for paraCL parser :
//                 statements -> statement; statements | empty 
//                  statement -> expression
//                 expression -> assignment | arithmetic_expr
//                 assignment -> variable | expression
//            arithmetic_expr -> arithmetic_expr bin_arithm_oper terminal
//                   terminal -> number | variable | ( arithmetic_expr )
//                   variable -> id 
//
//-------------------------------------------------------------------------------------------------

%language "c++"

%skeleton "lalr1.cc"
%defines
%define api.value.type variant
%param {yy::Driver* driver}

%code requires
{
#include <string>

namespace yy  
{
    class Driver;   //  <-- forvard decl of Driver
}   
}   //  requires

%code 
{
#include "driver.hpp"

namespace yy
{
    parser::token_type yylex( parser::semantic_type* yylval,                         
                              Driver* driver );
}
}   //  code

%token
    ASSIGN  "="
    MINUS   "-"
    PLUS    "+"
    DIV     "/"
    MUL     "*"
    LESS    "<"
    GREATER ">"
    EQUAL   "=="
    SCOLON  ";"
    LCBR    "{"
    RCBR    "}"
    LPAREN  "("
    RPAREN  ")"
    ERR     
;
%token <int> NUMBER
%token <std::string> ID
%nterm std:vector<StatementNode> statements 
%nterm StatementNode statement 
%nterm ExpressionNode expression 
%nterm AssignmentNode assignment 
%nterm ArithmeticNode arithmetic_expr

%left '-' '+'
%left '/' '*'


%start program

%%

program: statements { driver->insert($1); }
;

statements: statement SCOLON statements
          | statement SCOLON
;

statement: expression    
         | if_statement
         | while_statement
;

expression: assignment
          | arithmetic_expr
;

asssigment: variable
          | expression
;

arithmetic_expr: arithmetic_expr MINUS terminal
               | arithmetic_expr PLUS terminal
               | arithmetic_expr DIV terminal
               | arithmetic_expr MUL terminal
               | arithmetic_expr LESS terminal
               | arithmetic_expr GREATER terminal
               | arithmetic_expr EQUAL terminal
;

terminal: number    
        | variable
        | LPAREN arithmetic_expr RPAREN

%%

namespace yy 
{
    parser::token_type yylex( parser::semantic_type* yylval,                         
                              Driver* driver )
    {
        return driver->yylex(yylval);
    }

    void parser::error( //  arguments ....)
}