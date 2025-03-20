//-------------------------------------------------------------------------------------------------
//
//  Grammar for paraCL parser :
//      current_scope -> statement; current_scope 
//                       | empty 
//          statement -> expression 
//                       | if_expression 
//                       | while_expression 
//      if_expression -> if ( expression ) statement
//                       | if ( expression ) { current_scope }
//   while_expression -> while ( expression ) statement
//                       | while ( expression ) { current_scope }
//         expression -> assignment 
//                       | bin_op_exression
//                       | print 
//                       | ? 
//              print -> print expression
//                  ? -> input number
//         assignment -> variable = expression
//  bin_op_expression -> bin_op_expression bin_op subexpr 
//                       | subexpr
//            subexpr -> terminal 
//                       | ( expression )
//           terminal -> number 
//                       | variable 
//           variable -> id 
//
//-------------------------------------------------------------------------------------------------

%language "c++"

%skeleton "lalr1.cc"
%defines
%define api.value.type variant
%param {yy::Driver* driver}

%code requires
{
#include <iostream>
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
    LEQUAL
    GEQUAL
    NEQUAL
    SCOLON  
    LCBR
    RCBR    
    LPAREN  
    RPAREN  
    ERROR     
;

//  keywords
%token
    INPUT
    PRINT
    WHILE 
    IF
;

%token <int> NUMBER
%token <std::string> ID
%nterm <CurrentScopeNode*> current_scope 
%nterm <StatementINode*> statement 
%nterm <ExpressionWrapper*> expression_wrapper 
%nterm <ExpressionINode*> expression
%nterm <ExpressionINode*> subexpr
%nterm <IfExpressionNode*> if_expression
%nterm <WhileExpressionNode*> while_expression
%nterm <PrintNode*> print
%nterm <InputNode*> input
%nterm <AssignExpressionNode*> assignment 
%nterm <BinOpNode*> bin_op_expression
%nterm <ExpressionINode*> terminal
%nterm <VariableNode*> variable 

%left '-' '+'
%left '/' '*'


%start program

%%

program: current_scope  { driver->insert_current_scope($1);}
;

current_scope: statement SCOLON current_scope  { $$ = $3; $$.push_back($1); }
               | statement SCOLON              { $$.push_back($1); }
;

statement: expression_wrapper  { $$ = $1; }
         | if_expression       { $$ = $1; }
         | while_expression    { $$ = $1; }
;

if_expression: IF LPAREN expression RPAREN statement                { ScopeNode* scope = make_scope_node();
                                                                      scope->push_node_to_scope($5);
                                                                      $$ = make_if_node($3, scope); }
             | IF LPAREN expression RPAREN LCBR current_scope RCBR  { ScopeNode* scope = make_scope_node();
                                                                      scope->assign($6.begin(), $6.end());
                                                                      $$ = make_if_node($3, scope); }
;

while_expression: WHILE LPAREN expression RPAREN statement                { ScopeNode* scope = make_scope_node();
                                                                            scope->push_node_to_scope($5);
                                                                            $$ = make_while_node($3, scope); }
                | WHILE LPAREN expression RPAREN LCBR current_scope RCBR  { ScopeNode* scope = make_scope_node();
                                                                            scope->assign($6.begin(), $6.end());
                                                                            $$ = make_while_node($3, scope); }
;

expression_wrapper: expression  { $$ = $1; }
;

expression: assignment         { $$ = $1; }
          | bin_op_expression  { $$ = $1; }
          | print              { $$ = $1; }
          | input              { $$ = $1; }
;

print: PRINT expression { $$ = make_print_node($2); }
;

input: INPUT NUMBER  { $$ = make_input_node($2); }
;


assignment: variable ASSIGN expression  { BinOpNode* expr = make_bin_op_node($1, $3, BinOpType::ASSIGN);
                                          $$ = make_assignment_node(expr); }
;

bin_op_expression: bin_op_expression MINUS   subexpr   { BinOpNode* expr = make_bin_op_node($1, $3, BinOpType::MINUS);
                                                         $$ = make_arithmetic_node(expr); }
                 | bin_op_expression PLUS    subexpr   { BinOpNode* expr = make_bin_op_node($1, $3, BinOpType::PLUS); 
                                                         $$ = make_arithmetic_node(expr); }
                 | bin_op_expression DIV     subexpr   { BinOpNode* expr = make_bin_op_node($1, $3, BinOpType::DIV); 
                                                         $$ = make_arithmetic_node(expr); }
                 | bin_op_expression MUL     subexpr   { BinOpNode* expr = make_bin_op_node($1, $3, BinOpType::MUL); 
                                                         $$ = make_arithmetic_node(expr); }
                 | bin_op_expression LESS    subexpr   { BinOpNode* expr = make_bin_op_node($1, $3, BinOpType::LESS); 
                                                         $$ = make_arithmetic_node(expr); }
                 | bin_op_expression GREATER subexpr   { BinOpNode* expr = make_bin_op_node($1, $3, BinOpType::GREATER); 
                                                         $$ = make_arithmetic_node(expr); }
                 | bin_op_expression EQUAL   subexpr   { BinOpNode* expr = make_bin_op_node($1, $3, BinOpType::EQUAL); 
                                                         $$ = make_arithmetic_node(expr); }
                 | bin_op_expression LEQUAL   subexpr  { BinOpNode* expr = make_bin_op_node($1, $3, BinOpType::LEQUAL); 
                                                         $$ = make_arithmetic_node(expr); }
                 | bin_op_expression GEQUAL   subexpr  { BinOpNode* expr = make_bin_op_node($1, $3, BinOpType::GEQUAL); 
                                                         $$ = make_arithmetic_node(expr); }                      
                 | bin_op_expression NEQUAL   subexpr  { BinOpNode* expr = make_bin_op_node($1, $3, BinOpType::NEQUAL); 
                                                         $$ = make_arithmetic_node(expr); }                      
                 | subexpr                             { $$ = $1; }
;

subexpr: terminal                  { $$ = $1; }
       | LPAREN expression RPAREN  { $$ = $2; }
;

terminal: NUMBER    { $$ = make_number_node($1); }
        | variable  { $$ = $1; }
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

    void parser::error(const std::string&){}
}