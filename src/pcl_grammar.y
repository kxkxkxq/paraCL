//-------------------------------------------------------------------------------------------------
//
//      Current paraCL grammar 
//      (it will probably be modified and updated in the future) :
//             statements -> empty 
//                           | statements substmnt  
//               substmnt -> statement
//                           | { scope }
//                           | empty_statement;
//        empty_statement -> empty
//                  scope -> empty
//                           | scope substmnt
//              statement -> expression_wrapper; 
//                           | if_expression 
//                           | while_expression 
//          if_expression -> if ( expression ) substmnt
//       while_expression -> while ( expression ) substmnt
//     expression_wrapper -> expression
//             expression -> assignment 
//                           | arithmetic_exression
//                           | print 
//             assignment -> variable = expression
//                           | variable = input
//                  input -> ?
//                  print -> print expression
//  arithmetic_expression -> arithmetic_expression bin_op arithmetic_expression 
//                           | subexpr
//                subexpr -> terminal 
//                           | ( expression )
//               terminal -> number 
//                           | variable 
//               variable -> id 
//
//-------------------------------------------------------------------------------------------------
%language "c++"

%skeleton "lalr1.cc"
%defines
%define api.value.type variant
%param {yy::Driver* driver}

%code requires
{
#include <cassert>
#include <iostream>
#include <string>

#include "node.hpp"

using namespace ast;

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

CurrentScopeNode* currScope = nullptr;
}   //  code

%token
    ASSIGN  
    MINUS  
    PLUS    
    DIV     
    MUL     
    LESS    
    GREATER
    MOD 
    EQUAL
    LEQUAL
    GEQUAL
    NEQUAL
    AND
    OR
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
%nterm <EmptyStatement*> empty_statement
%nterm <NumberNode*> number
%nterm <CurrentScopeNode*> statements 
%nterm <CurrentScopeNode*> scope
%nterm <StatementINode*> statement 
%nterm <StatementWrapper*> substmnt
%nterm <ExpressionWrapper*> expression_wrapper 
%nterm <ArithmExprWrapper*> arithmetic_expression
%nterm <ExpressionINode*> expression
%nterm <ExpressionINode*> subexpr
%nterm <IfExpressionNode*> if_expression
%nterm <WhileExpressionNode*> while_expression
%nterm <PrintNode*> print
%nterm <InputNode*> input
%nterm <AssignExpressionNode*> assignment 
%nterm <ExpressionINode*> terminal
%nterm <VariableNode*> variable 

%left ASSIGN
%left AND OR
%left LESS GREATER EQUAL LEQUAL GEQUAL NEQUAL
%left MINUS PLUS
%left DIV MUL MOD
%left UMINUS


%start program

%%
program: statements  { driver->set_ast_root($1); }
;

statements: %empty               { $$ = driver->make_node<CurrentScopeNode>(); 
                                   assert($$);
                                   driver->descend_into_scope($$); 
                                   assert($$); }
          | statements substmnt  { assert($1);
                                   assert($2);
                                   $$ = $1;
                                   $$->add_statement($2); }
;

substmnt: statement               { $$ = driver->make_node<StatementWrapper>($1); }
        | LCBR scope RCBR         { $$ = driver->make_node<StatementWrapper>($2);
                                    driver->ascend_from_scope(); }
        | empty_statement SCOLON  { $$ = driver->make_node<StatementWrapper>($1); }
;

empty_statement: %empty  { $$ = driver->make_node<EmptyStatement>(); }
;

scope: %empty          { $$ = driver->make_node<CurrentScopeNode>();
                         driver->descend_into_scope($$); }
     | scope substmnt  { assert($1);
                         assert($2);
                         $$ = $1;
                         $$->add_statement($2); }
;

statement: expression_wrapper SCOLON  { $$ = $1; }
         | if_expression              { $$ = $1; }
         | while_expression           { $$ = $1; }
;

if_expression: IF LPAREN expression RPAREN substmnt  { $$ = driver->make_node<IfExpressionNode>($3, $5); } 
;

while_expression: WHILE LPAREN expression RPAREN substmnt  { $$ = driver->make_node<WhileExpressionNode>($3, $5); } 
;

expression_wrapper: expression  { $$ = driver->make_node<ExpressionWrapper>($1); }
;

expression: assignment             { $$ = $1; }
          | arithmetic_expression  { $$ = $1; }
          | print                  { $$ = $1; }
;

assignment: variable ASSIGN expression  { $$ = driver->make_node<AssignExpressionNode>($1, $3);
                                          driver->add_to_context($1); }
          | variable ASSIGN input       { $$ = driver->make_node<AssignExpressionNode>($1, $3); }
;

input: INPUT { NumberNode* number = driver->make_node<NumberNode>();
               $$ = driver->make_node<InputNode>(number); }
;

print: PRINT expression { $$ = driver->make_node<PrintNode>($2); }
;

arithmetic_expression: arithmetic_expression MINUS arithmetic_expression  %prec MINUS  
                       { 
                            BinOpNode* expr = driver->make_node<BinOpNode>($1, $3, ast::BinOpType::MINUS);
                            $$ = driver->make_node<ArithmExprWrapper>(std::move(expr)); 
                       } 
                     | arithmetic_expression PLUS arithmetic_expression %prec PLUS    
                       { 
                            BinOpNode* expr = driver->make_node<BinOpNode>($1, $3, ast::BinOpType::PLUS);
                            $$ = driver->make_node<ArithmExprWrapper>(std::move(expr)); 
                       }
                     | arithmetic_expression DIV arithmetic_expression %prec DIV      
                       { 
                            BinOpNode* expr = driver->make_node<BinOpNode>($1, $3, ast::BinOpType::DIV);
                            $$ = driver->make_node<ArithmExprWrapper>(std::move(expr)); 
                       } 
                     | arithmetic_expression MUL arithmetic_expression %prec MUL      
                       { 
                            BinOpNode* expr = driver->make_node<BinOpNode>($1, $3, ast::BinOpType::MUL);
                            $$ = driver->make_node<ArithmExprWrapper>(std::move(expr)); 
                       } 
                     | arithmetic_expression LESS arithmetic_expression %prec LESS     
                       { 
                            BinOpNode* expr = driver->make_node<BinOpNode>($1, $3, ast::BinOpType::LESS);
                            $$ = driver->make_node<ArithmExprWrapper>(std::move(expr));
                       } 
                     | arithmetic_expression GREATER arithmetic_expression %prec GREATER
                       { 
                            BinOpNode* expr = driver->make_node<BinOpNode>($1, $3, ast::BinOpType::GREATER);
                            $$ = driver->make_node<ArithmExprWrapper>(std::move(expr));
                       }
                     | arithmetic_expression MOD arithmetic_expression %prec MOD
                       { 
                            BinOpNode* expr = driver->make_node<BinOpNode>($1, $3, ast::BinOpType::MOD);
                            $$ = driver->make_node<ArithmExprWrapper>(std::move(expr)); 
                       }  
                     | arithmetic_expression EQUAL arithmetic_expression %prec EQUAL   
                       { 
                            BinOpNode* expr = driver->make_node<BinOpNode>($1, $3, ast::BinOpType::EQUAL);
                            $$ = driver->make_node<ArithmExprWrapper>(std::move(expr)); 
                       } 
                     | arithmetic_expression LEQUAL arithmetic_expression %prec LEQUAL
                       { 
                            BinOpNode* expr = driver->make_node<BinOpNode>($1, $3, ast::BinOpType::LEQUAL);
                            $$ = driver->make_node<ArithmExprWrapper>(std::move(expr)); 
                       } 
                     | arithmetic_expression GEQUAL arithmetic_expression %prec GEQUAL
                       { 
                            BinOpNode* expr = driver->make_node<BinOpNode>($1, $3, ast::BinOpType::GEQUAL);
                            $$ = driver->make_node<ArithmExprWrapper>(std::move(expr)); 
                       } 
                     | arithmetic_expression NEQUAL arithmetic_expression %prec NEQUAL
                       { 
                            BinOpNode* expr = driver->make_node<BinOpNode>($1, $3, ast::BinOpType::NEQUAL);
                            $$ = driver->make_node<ArithmExprWrapper>(std::move(expr)); 
                       }   
                     | arithmetic_expression AND arithmetic_expression %prec AND
                       { 
                            BinOpNode* expr = driver->make_node<BinOpNode>($1, $3, ast::BinOpType::AND);
                            $$ = driver->make_node<ArithmExprWrapper>(std::move(expr)); 
                       }   
                     | arithmetic_expression OR arithmetic_expression %prec OR
                       { 
                            BinOpNode* expr = driver->make_node<BinOpNode>($1, $3, ast::BinOpType::OR);
                            $$ = driver->make_node<ArithmExprWrapper>(std::move(expr)); 
                       }                      
                     | MINUS subexpr %prec UMINUS  
                       { 
                            $$ = driver->make_node<ArithmExprWrapper>($2, ast::BinOpType::MINUS); 
                       }
                     | subexpr  { $$ = driver->make_node<ArithmExprWrapper>($1); }
;

subexpr: terminal                  { $$ = $1; }
       | LPAREN expression RPAREN  { $$ = $2; }
;


terminal: number    { $$ = $1; }
        | variable  { $$ = $1; }
;

number: NUMBER { $$ = driver->make_node<NumberNode>($1); }
;

variable: ID  { $$ = driver->make_or_assign<VariableNode>($1); 
                driver->add_to_context($$); } 
;

%%

namespace yy 
{
     parser::token_type yylex( parser::semantic_type* yylval,                         
                              Driver* driver )
     {
          return driver->yylex(yylval);
     }

     void parser::error(const std::string&){ std::cerr << " ...." << std::endl; }
}