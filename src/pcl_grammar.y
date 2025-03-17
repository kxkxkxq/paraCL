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
    ERR     
;

//  keywords
%token
    INPUT
    OUTPUT
    WHILE 
    IF
;

%token <int> NUMBER
%token <std::string> ID
%nterm <Statements> statements 
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

arithmetic_expr: arithmetic_expr MINUS   terminal  { BinOpNode* binOpExpr = make_bin_op_node($1, $3, BinOpType::MINUS);
                                                     $$ = make_expression_node(binOpExpr); }
               | arithmetic_expr PLUS    terminal  { BinOpNode* binOpExpr = make_bin_op_node($1, $3, BinOpType::PLUS); 
                                                     $$ = make_expression_node(binOpExpr); }
               | arithmetic_expr DIV     terminal  { BinOpNode* binOpExpr = make_bin_op_node($1, $3, BinOpType::DIV); 
                                                     $$ = make_expression_node(binOpExpr); }
               | arithmetic_expr MUL     terminal  { BinOpNode* binOpExpr = make_bin_op_node($1, $3, BinOpType::MUL); 
                                                     $$ = make_expression_node(binOpExpr); }
               | arithmetic_expr LESS    terminal  { BinOpNode* binOpExpr = make_bin_op_node($1, $3, BinOpType::LESS); 
                                                     $$ = make_expression_node(binOpExpr); }
               | arithmetic_expr GREATER terminal  { BinOpNode* binOpExpr = make_bin_op_node($1, $3, BinOpType::GREATER); 
                                                     $$ = make_expression_node(binOpExpr); }
               | arithmetic_expr EQUAL   terminal  { BinOpNode* binOpExpr = make_bin_op_node($1, $3, BinOpType::EQUAL); 
                                                     $$ = make_expression_node(binOpExpr); }
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