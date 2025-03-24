//-------------------------------------------------------------------------------------------------
//
//  Current paraCL grammar 
//  (it will probably be modified and updated in the future) :
//      current_scope -> statement; current_scope 
//                       | { current_scope }
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

%left '-' '+'
%left '/' '*'


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

substmnt: statement SCOLON  { $$ = driver->make_node<StatementWrapper>($1); }
        | LCBR scope   RCBR {  $$ = driver->make_node<StatementWrapper>($2); }
;

scope: %empty          { $$ = driver->make_node<CurrentScopeNode>(); }
     | scope substmnt  { assert($1);
                         assert($2);
                         $$ = $1;
                         $$->add_statement($2); }
;

statement: expression_wrapper  { $$ = $1; }
;

//-------------------------------------------------------------------------------------------------
    //  obsolete
/*
if_expression: IF LPAREN expression RPAREN scope  {  $5 = driver->make_node<CurrentScopeNode>();
                                                     driver->descend_into_scope($5);
                                                     $$ = driver->make_node<IfExpressionNode>($3, $5); 
                                                     driver->ascend_from_scope(); }
;

scope: %empty                       { $$ = driver->make_node<CurrentScopeNode>(); 
                                      assert($$);
                                      driver->descend_into_scope($$); 
                                      assert($$); }
     | statement SCOLON scope       { $$ = $3;
                                      assert($$); 
                                      $$->add_statement($1);
                                      assert($$); }
                            
;
*/
//-------------------------------------------------------------------------------------------------






expression_wrapper: expression  { $$ = driver->make_node<ExpressionWrapper>($1); }
;


expression: assignment             { $$ = $1; }
          | arithmetic_expression  { $$ = $1; }
          | print                  { $$ = $1; }
;

assignment: variable ASSIGN expression  { $$ = driver->make_node<AssignExpressionNode>($1, $3); }
          | variable ASSIGN input       { $$ = driver->make_node<AssignExpressionNode>($1, $3); }
;

input: INPUT number { $$ = driver->make_node<InputNode>($2); }
;

print: PRINT expression { $$ = driver->make_node<PrintNode>($2); }
;

arithmetic_expression: arithmetic_expression MINUS subexpr    { BinOpNode* expr = driver->make_node<BinOpNode>($1, $3, ast::BinOpType::MINUS);
                                                                $$ = driver->make_node<ArithmExprWrapper>(std::move(expr)); } 
                     | arithmetic_expression PLUS subexpr     { BinOpNode* expr = driver->make_node<BinOpNode>($1, $3, ast::BinOpType::PLUS);
                                                                $$ = driver->make_node<ArithmExprWrapper>(std::move(expr)); }
                     | arithmetic_expression DIV subexpr      { BinOpNode* expr = driver->make_node<BinOpNode>($1, $3, ast::BinOpType::DIV);
                                                                $$ = driver->make_node<ArithmExprWrapper>(std::move(expr)); } 
                     | arithmetic_expression MUL subexpr      { BinOpNode* expr = driver->make_node<BinOpNode>($1, $3, ast::BinOpType::MUL);
                                                                $$ = driver->make_node<ArithmExprWrapper>(std::move(expr)); } 
                     | arithmetic_expression LESS subexpr     { BinOpNode* expr = driver->make_node<BinOpNode>($1, $3, ast::BinOpType::LESS);
                                                                $$ = driver->make_node<ArithmExprWrapper>(std::move(expr)); } 
                     | arithmetic_expression GREATER subexpr  { BinOpNode* expr = driver->make_node<BinOpNode>($1, $3, ast::BinOpType::GREATER);
                                                                $$ = driver->make_node<ArithmExprWrapper>(std::move(expr)); } 
                     | arithmetic_expression EQUAL subexpr    { BinOpNode* expr = driver->make_node<BinOpNode>($1, $3, ast::BinOpType::EQUAL);
                                                                $$ = driver->make_node<ArithmExprWrapper>(std::move(expr)); } 
                     | arithmetic_expression LEQUAL subexpr   { BinOpNode* expr = driver->make_node<BinOpNode>($1, $3, ast::BinOpType::LEQUAL);
                                                                $$ = driver->make_node<ArithmExprWrapper>(std::move(expr)); } 
                     | arithmetic_expression GEQUAL subexpr   { BinOpNode* expr = driver->make_node<BinOpNode>($1, $3, ast::BinOpType::GEQUAL);
                                                                $$ = driver->make_node<ArithmExprWrapper>(std::move(expr)); } 
                     | arithmetic_expression NEQUAL subexpr   { BinOpNode* expr = driver->make_node<BinOpNode>($1, $3, ast::BinOpType::NEQUAL);
                                                                $$ = driver->make_node<ArithmExprWrapper>(std::move(expr)); }                      
                     | subexpr                                { $$ = driver->make_node<ArithmExprWrapper>($1); }
;

subexpr: terminal                  { $$ = $1; }
       | LPAREN expression RPAREN  { $$ = $2; }
;


terminal: number    { $$ = $1; }
        | variable  { $$ = $1; }
;

number: NUMBER { $$ = driver->make_node<NumberNode>($1); }
;

variable: ID  { $$ = driver->make_node<VariableNode>($1); }
;













//-------------------------------------------------------------------------------------------------
    //  obsolete
/*
program: statements  { driver->insert_current_scope($1);}
;

statements: statement SCOLON statements  { $$ = $3; $$->add_statement($1); }
               //| LCBR current_scope RCBR       { $$ = $2; }
               | statement SCOLON              { std::cout << "2"; $$->add_statement($1); }
;

statement: expression_wrapper      { $$ = $1; }
         | if_expression           { $$ = $1; }
         | while_expression        { $$ = $1; }
;

if_expression: IF LPAREN expression RPAREN scope  { 
                                                     CurrentScopeNode* scope = make_node<CurrentScopeNode>(nullptr);
                                                     $$ = make_node<IfExpressionNode>($3, scope); 
                                                  }
;



while_expression: WHILE LPAREN expression RPAREN statement                { 
                                                                            CurrentScopeNode* scope = make_node<CurrentScopeNode>(nullptr);        
                                                                            $$ = make_node<WhileExpressionNode>($3, scope);
                                                                          }
;

scope: statement             { std::cout << "scope" << std::endl; }
     | LCBR statements RCBR  {  std::cout << "scope" << std::endl; }

expression_wrapper: expression  { $$ = make_node<ExpressionWrapper>($1); }
;

expression: assignment             { $$ = $1; }
          | arithmetic_expression  { $$ = $1; }
          | print                  { $$ = $1; }
          | input                  { $$ = $1; }
          | terminal               { $$ = $1; }
;

print: PRINT expression { $$ = make_node<PrintNode>($2); }
;

input: INPUT NUMBER  { $$ = make_node<InputNode>(make_node<NumberNode>($2)); }
;


assignment: variable ASSIGN expression  { $$ = make_node<AssignExpressionNode>($1, $3); }
;

arithmetic_expression: arithmetic_expression MINUS   subexpr   { $$ = make_node<BinOpNode>($1, $3, BinOpType::MINUS); }
                     | arithmetic_expression PLUS    subexpr   { $$ = make_node<BinOpNode>($1, $3, BinOpType::PLUS); }
                     | arithmetic_expression DIV     subexpr   { $$ = make_node<BinOpNode>($1, $3, BinOpType::DIV); }
                     | arithmetic_expression MUL     subexpr   { $$ = make_node<BinOpNode>($1, $3, BinOpType::MUL); }
                     | arithmetic_expression LESS    subexpr   { $$ = make_node<BinOpNode>($1, $3, BinOpType::LESS); }
                     | arithmetic_expression GREATER subexpr   { $$ = make_node<BinOpNode>($1, $3, BinOpType::GREATER); }
                     | arithmetic_expression EQUAL   subexpr   { $$ = make_node<BinOpNode>($1, $3, BinOpType::EQUAL); }
                     | arithmetic_expression LEQUAL   subexpr  { $$ = make_node<BinOpNode>($1, $3, BinOpType::LEQUAL); }
                     | arithmetic_expression GEQUAL   subexpr  { $$ = make_node<BinOpNode>($1, $3, BinOpType::GEQUAL); }                      
                     | arithmetic_expression NEQUAL   subexpr  { $$ = make_node<BinOpNode>($1, $3, BinOpType::NEQUAL); } 
                     | subexpr                                 { $$ = $1; }                   


subexpr: terminal                             { $$ = $1; }
       | LPAREN arithmetic_expression RPAREN  { $$ = $2; }
;

terminal: NUMBER    { $$ = make_node<NumberNode>($1); }
        | variable  { $$ = $1; }
;

variable: ID  { $$ = make_node<VariableNode>($1); }
;
*/
//-------------------------------------------------------------------------------------------------

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