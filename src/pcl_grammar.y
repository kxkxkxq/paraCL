//-------------------------------------------------------------------------------------------------
//
//  Grammar for paraCL parser :
//                 statements -> statement; statements | empty 
//                  statement -> expression | if_expression | while_expression 
//                 expression -> assignment | arithmetic_expr
//                 assignment -> variable ASSIGN expression
//            arithmetic_expr -> arithmetic_expr bin_arithm_oper terminal | terminal
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

#include "node.hpp"

using namespace astnodes;

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
    ASSIGN  
    MINUS   
    PLUS    
    DIV     
    MUL     
    LESS    
    GREATER 
    EQUAL   
    SCOLON  
    LCBR
    RCBR    
    LPAREN  
    RPAREN  
    WHILE 
    IF
    ERR     
;

%token <int> NUMBER
%token <std::string> ID
%nterm <std::vector<StatementNode*>> statements 
%nterm <StatementNode*> statement 
%nterm <ExpressionNode*> expression 
%nterm <IfExpressionNode*> if_expression
%nterm <WhileExpressionNode*> while_expression
%nterm <ScopeNode*> scope
%nterm <ExpressionNode*> assignment 
%nterm <ExpressionNode*> arithmetic_expr
%nterm <INode*> variable
%nterm <Inode*> terminal 

%left '-' '+'
%left '/' '*'


%start program

%%

program: statements  { driver->insert_statements($1); }
;

statements: statement SCOLON statements  { $$ = $3; $$.push_back($1); }
          | statement SCOLON             { $$.push_back($1); }
;

statement: expression        { $$ = $1; }
         | if_expression     { $$ = $1; }
         | while_expression  { $$ = $1; }
;

if_expression: IF LPAREN expression RPAREN statement             {}
             | IF LPAREN expression RPAREN LCBR statements RCBR  {}
;

while_expression: WHILE LPAREN expression RPAREN statement             {}
                | WHILE LPAREN expression RPAREN LCBR statements RCBR  {}
;

expression: assignment       { $$ = $1; }
          | arithmetic_expr  { $$ = $1; }
;

assignment: variable ASSIGN expression  { BinOpNode* binOpExpr = make_bin_op_node($1, $3, BinOpType::ASSIGN);
                                          $$ = make_expression_node(binOpExpr); }
;

arithmetic_expr: arithmetic_expr MINUS   terminal  { $$ = make_bin_op_node($1, $3, BinOpType::MINUS); }
               | arithmetic_expr PLUS    terminal  { $$ = make_bin_op_node($1, $3, BinOpType::PLUS); }
               | arithmetic_expr DIV     terminal  { $$ = make_bin_op_node($1, $3, BinOpType::DIV); }
               | arithmetic_expr MUL     terminal  { $$ = make_bin_op_node($1, $3, BinOpType::MUL); }
               | arithmetic_expr LESS    terminal  { $$ = make_bin_op_node($1, $3, BinOpType::LESS); }
               | arithmetic_expr GREATER terminal  { $$ = make_bin_op_node($1, $3, BinOpType::GREATER); }
               | arithmetic_expr EQUAL   terminal  { $$ = make_bin_op_node($1, $3, BinOpType::EQUAL); }
;

terminal: NUMBER                         { $$ = make_number_node($1); }
        | variable                       { $$ = $1; }
        | LPAREN arithmetic_expr RPAREN  { $$ = $2; }
;

variable: ID  { $$ = make_id_node($1); }
;

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