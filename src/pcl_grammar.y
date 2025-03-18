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
%nterm <StatementINode*> statement 
%nterm <ExpressionINode*> expression 
%nterm <ExpressionINode*> subexpr
%nterm <IfExprNode*> if_expression
%nterm <WhileExprNode*> while_expression
%nterm <ScopeNode*> scope
%nterm <AssignmentExprNode*> assignment 
%nterm <ArithmeticExprNode*> arithmetic_expr
%nterm <TerminalINode*> terminal
%nterm <VariableINode*> variable 

%left '-' '+'
%left '/' '*'


%start program

%%

program: statements  { driver->insert_statements($1); }
;

statements: statement SCOLON statements  { $$ = $3; $$.add_statement($1); }
          | statement SCOLON             { $$.add_statement($1); }
;

statement: expression        { $$ = $1; }
         | if_expression     { $$ = $1; }
         | while_expression  { $$ = $1; }
;

if_expression: IF LPAREN expression RPAREN statement             { ScopeNode* scope = make_scope_node();
                                                                   scope->push_node_to_scope($5);
                                                                   $$ = make_if_node($3, scope); }
             | IF LPAREN expression RPAREN LCBR statements RCBR  { ScopeNode* scope = make_scope_node();
                                                                   scope->assign($6.begin(), $6.end());
                                                                   $$ = make_if_node($3, scope); }
;

while_expression: WHILE LPAREN expression RPAREN statement             { ScopeNode* scope = make_scope_node();
                                                                         scope->push_node_to_scope($5);
                                                                         $$ = make_while_node($3, scope); }
                | WHILE LPAREN expression RPAREN LCBR statements RCBR  { ScopeNode* scope = make_scope_node();
                                                                         scope->assign($6.begin(), $6.end());
                                                                         $$ = make_while_node($3, scope); }
;

expression: assignment       { $$ = $1; }
          | arithmetic_expr  { $$ = $1; }
;

assignment: variable ASSIGN expression  { BinOpNode* expr = make_bin_op_node($1, $3, BinOpType::ASSIGN);
                                          $$ = make_assignment_node(expr); }
;

arithmetic_expr: arithmetic_expr MINUS   subexpr  { BinOpNode* expr = make_bin_op_node($1, $3, BinOpType::MINUS);
                                                     $$ = make_arithmetic_node(expr); }
               | arithmetic_expr PLUS    subexpr  { BinOpNode* expr = make_bin_op_node($1, $3, BinOpType::PLUS); 
                                                     $$ = make_arithmetic_node(expr); }
               | arithmetic_expr DIV     subexpr  { BinOpNode* expr = make_bin_op_node($1, $3, BinOpType::DIV); 
                                                     $$ = make_arithmetic_node(expr); }
               | arithmetic_expr MUL     subexpr  { BinOpNode* expr = make_bin_op_node($1, $3, BinOpType::MUL); 
                                                     $$ = make_arithmetic_node(expr); }
               | arithmetic_expr LESS    subexpr  { BinOpNode* expr = make_bin_op_node($1, $3, BinOpType::LESS); 
                                                     $$ = make_arithmetic_node(expr); }
               | arithmetic_expr GREATER subexpr  { BinOpNode* expr = make_bin_op_node($1, $3, BinOpType::GREATER); 
                                                     $$ = make_arithmetic_node(expr); }
               | arithmetic_expr EQUAL   subexpr  { BinOpNode* expr = make_bin_op_node($1, $3, BinOpType::EQUAL); 
                                                     $$ = make_arithmetic_node(expr); }
               | subexpr                          { $$ = $1; }
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

    void parser::error( //  arguments ....)
}