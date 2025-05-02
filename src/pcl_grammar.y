//-------------------------------------------------------------------------------------------------
//
// Current paraCL grammar 
// (it will probably be modified and updated in the future) :
//             statements -> empty 
//                           | statements stmnt_wrapper
//          stmnt_wrapper -> substmnt
//                           | { scope }  
//               substmnt -> statement
//                           | empty_statement;
//                           | error;
//        empty_statement -> empty
//                  scope -> empty
//                           | scope stmnt_wrapper
//              statement -> expression_wrapper; 
//                           | if_expression 
//                           | while_expression 
//          if_expression -> if ( expression ) 
//                             scope_wrapper
//                           | if ( expression ) 
//                               scope_wrapper 
//                             else 
//                               scope_wrapper
//       while_expression -> while ( expression ) scope_wrapper
//     expression_wrapper -> expression
//             expression -> assignment 
//                           | algebraic_expression
//                           | print 
//             assignment -> variable = expression
//                  input -> ?
//                  print -> print expression
//   algebraic_expression -> arithmetic_expression
//                           | logic_expression 
//                           | subexpr
//  arithmetic_expression -> algebraic_expression bin_op algebraic_expression
//                           | -subexpr
//                           | +subexpr
//       logic_expression -> algebraic_expression bin_op algebraic_expression
//                           | !subexpr            
//                subexpr -> terminal 
//                           | ( expression )
//                           | input
//               terminal -> number 
//                           | variable 
//               variable -> id 
//
//-------------------------------------------------------------------------------------------------
%language "c++"

%skeleton "lalr1.cc"
%defines
%define api.value.type variant

%define parse.lac full
%define parse.error verbose
%locations

%param {yy::Driver* driver}

%code requires
{
#include <cassert>
#include <iostream>
#include <string>

#include "error_report.hpp"
#include "node.hpp"

using namespace ast;

namespace yy  
{
    class Driver; 
}  
}  //  requires

%code 
{
#include "driver.hpp"

namespace yy
{
     parser::token_type yylex( parser::semantic_type* yylval,                         
                               parser::location_type* loc,
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
    MOD 
    EQUAL
    LEQUAL
    GEQUAL
    NEQUAL
    AND
    OR
    NOT
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
    ELSE
;

%token <int> NUMBER
%token <std::string> ID
%nterm <EmptyStatement*> empty_statement
%nterm <NumberNode*> number
%nterm <CurrentScopeNode*> statements 
%nterm <CurrentScopeNode*> scope
%nterm <CurrentScopeNode*> subscope
%nterm <StatementINode*> statement 
%nterm <StatementWrapper*> stmnt_wrapper
%nterm <StatementWrapper*> scope_wrapper
%nterm <StatementINode*> substmnt
%nterm <ExpressionWrapper*> expression_wrapper 
%nterm <ExpressionINode*> algebraic_expression
%nterm <ArithmExprNode*> arithmetic_expression
%nterm <LogicExprNode*> logic_expression
%nterm <ExpressionINode*> expression
%nterm <ExpressionINode*> subexpr
%nterm <IfExpressionNode*> if_expression
%nterm <WhileExpressionNode*> while_expression
%nterm <PrintNode*> print
%nterm <InputNode*> input
%nterm <AssignExpressionNode*> assignment 
%nterm <ExpressionINode*> terminal
%nterm <VariableNode*> variable 

%right ASSIGN
%right AND OR
%left LESS GREATER EQUAL LEQUAL GEQUAL NEQUAL
%left MINUS PLUS
%right DIV MUL MOD
%left NOT
%left UMINUS UPLUS

%nonassoc IF_WITHOUT_ELSE
%nonassoc ELSE

%nonassoc LOGIC
%nonassoc ARITHM

%start program

%%
program: statements  { driver->set_ast_root($1); }
;

statements: %empty                    { 
                                        $$ = driver->make_node<CurrentScopeNode>(); 
                                        assert($$);
                                        driver->descend_into_scope($$); 
                                        assert($$); 
                                      }
          | statements stmnt_wrapper  { 
                                        assert($1);
                                        assert($2);
                                        $$ = $1;
                                        $$->add_statement($2); 
                                      }
;

stmnt_wrapper: substmnt         { $$ = driver->make_node<StatementWrapper>($1); }
             | LCBR scope RCBR  { 
                                  $$ = driver->make_node<StatementWrapper>($2);
                                  driver->ascend_from_scope();
                                }
;

substmnt: statement               { $$ = driver->make_node<StatementWrapper>($1); }
        | empty_statement SCOLON  { $$ = driver->make_node<StatementWrapper>($1); }
        | error SCOLON            { /* error reporting */ }
;

empty_statement: %empty  { $$ = driver->make_node<EmptyStatement>(); }
;

scope: %empty               { 
                              $$ = driver->make_node<CurrentScopeNode>();
                              driver->descend_into_scope($$); 
                            }
     | scope stmnt_wrapper  { 
                              assert($1);
                              assert($2);
                              $$ = $1;
                              $$->add_statement($2); 
                            }
;

statement: expression_wrapper SCOLON  { $$ = $1; }
         | if_expression              { $$ = $1; }
         | while_expression           { $$ = $1; }
;

if_expression: IF LPAREN expression RPAREN 
                  scope_wrapper %prec IF_WITHOUT_ELSE  { $$ = driver->make_node<IfExpressionNode>($3, $5); } 
             | IF LPAREN expression RPAREN 
                  scope_wrapper 
               ELSE 
                  scope_wrapper  { $$ = driver->make_node<IfExpressionNode>($3, $5, $7); }
;

scope_wrapper: LCBR scope RCBR    { $$ = driver->make_node<StatementWrapper>($2); driver->ascend_from_scope(); }
             | subscope substmnt  { 
                                    $1->add_statement($2);
                                    $$ = driver->make_node<StatementWrapper>($1); 
                                    driver->ascend_from_scope();
                                  }
;  

subscope: %empty  { 
                    $$ = driver->make_node<CurrentScopeNode>();
                    driver->descend_into_scope($$);
                  }

while_expression: WHILE LPAREN expression RPAREN scope_wrapper  { $$ = driver->make_node<WhileExpressionNode>($3, $5); } 
;

expression_wrapper: expression  { $$ = driver->make_node<ExpressionWrapper>($1); }
;

expression: assignment            { $$ = $1; }
          | algebraic_expression  { $$ = $1; }
          | print                 { $$ = $1; }
;

assignment: variable ASSIGN expression  { 
                                          $$ = driver->make_node<AssignExpressionNode>($1, $3);
                                          driver->add_to_context($1);
                                        }
;

input: INPUT  { 
                NumberNode* number = driver->make_node<NumberNode>();
                $$ = driver->make_node<InputNode>(number); 
              }
;

print: PRINT expression  { $$ = driver->make_node<PrintNode>($2); }
;

algebraic_expression: arithmetic_expression %prec ARITHM  { $$ = $1; }
                    | logic_expression      %prec LOGIC   { $$ = $1; }
                    | subexpr                             { $$ = $1; }
;

arithmetic_expression: algebraic_expression MINUS algebraic_expression %prec MINUS
                       {
                         using ArithmBinOp = BinOpNode<ast::ArithmOpType>;
                         auto expr = driver->make_node<ArithmBinOp>($1, $3, ast::ArithmOpType::MINUS);
                         $$ = driver->make_node<ArithmExprNode>(std::move(expr));       
                       }
                     | algebraic_expression PLUS algebraic_expression %prec PLUS
                       {
                         using ArithmBinOp = BinOpNode<ast::ArithmOpType>;
                         auto expr = driver->make_node<ArithmBinOp>($1, $3, ast::ArithmOpType::PLUS);
                         $$ = driver->make_node<ArithmExprNode>(std::move(expr));       
                       }
                     | algebraic_expression DIV algebraic_expression %prec DIV
                       {
                         using ArithmBinOp = BinOpNode<ast::ArithmOpType>;
                         auto expr = driver->make_node<ArithmBinOp>($1, $3, ast::ArithmOpType::DIV);
                         $$ = driver->make_node<ArithmExprNode>(std::move(expr));       
                       }
                     | algebraic_expression MUL algebraic_expression %prec MUL
                       {
                         using ArithmBinOp = BinOpNode<ast::ArithmOpType>;
                         auto expr = driver->make_node<ArithmBinOp>($1, $3, ast::ArithmOpType::MUL);
                         $$ = driver->make_node<ArithmExprNode>(std::move(expr));       
                       }
                     | algebraic_expression MOD algebraic_expression %prec MOD
                       {
                         using ArithmBinOp = BinOpNode<ast::ArithmOpType>;
                         auto expr = driver->make_node<ArithmBinOp>($1, $3, ast::ArithmOpType::MOD);
                         $$ = driver->make_node<ArithmExprNode>(std::move(expr));       
                       } 
                     | MINUS subexpr %prec UMINUS 
                       { 
                         $$ = driver->make_node<ArithmExprNode>($2, ast::ArithmOpType::UMINUS); 
                       } 
                     | PLUS subexpr %prec UPLUS 
                       { 
                         $$ = driver->make_node<ArithmExprNode>($2, ast::ArithmOpType::UPLUS); 
                       } 
;

logic_expression: algebraic_expression LESS algebraic_expression %prec LESS
                  {
                    using LogicBinOp = BinOpNode<ast::LogicOpType>;
                    auto expr = driver->make_node<LogicBinOp>($1, $3, ast::LogicOpType::LESS);
                    $$ = driver->make_node<LogicExprNode>(std::move(expr), ast::LogicOpType::LESS);       
                  }
                | algebraic_expression GREATER algebraic_expression %prec GREATER
                  {
                    using LogicBinOp = BinOpNode<ast::LogicOpType>;
                    auto expr = driver->make_node<LogicBinOp>($1, $3, ast::LogicOpType::GREATER);
                    $$ = driver->make_node<LogicExprNode>(std::move(expr), ast::LogicOpType::GREATER);       
                  }
                | algebraic_expression EQUAL algebraic_expression %prec EQUAL
                  {
                    using LogicBinOp = BinOpNode<ast::LogicOpType>;
                    auto expr = driver->make_node<LogicBinOp>($1, $3, ast::LogicOpType::EQUAL);
                    $$ = driver->make_node<LogicExprNode>(std::move(expr), ast::LogicOpType::EQUAL);       
                  }
                | algebraic_expression LEQUAL algebraic_expression %prec LEQUAL
                  {
                    using LogicBinOp = BinOpNode<ast::LogicOpType>;
                    auto expr = driver->make_node<LogicBinOp>($1, $3, ast::LogicOpType::LEQUAL);
                    $$ = driver->make_node<LogicExprNode>(std::move(expr), ast::LogicOpType::LEQUAL);       
                  }
                | algebraic_expression GEQUAL algebraic_expression %prec GEQUAL
                  {
                    using LogicBinOp = BinOpNode<ast::LogicOpType>;
                    auto expr = driver->make_node<LogicBinOp>($1, $3, ast::LogicOpType::GEQUAL);
                    $$ = driver->make_node<LogicExprNode>(std::move(expr), ast::LogicOpType::GEQUAL);       
                  }
                | algebraic_expression NEQUAL algebraic_expression %prec NEQUAL
                  {
                    using LogicBinOp = BinOpNode<ast::LogicOpType>;
                    auto expr = driver->make_node<LogicBinOp>($1, $3, ast::LogicOpType::NEQUAL);
                    $$ = driver->make_node<LogicExprNode>(std::move(expr), ast::LogicOpType::NEQUAL);       
                  }
                | algebraic_expression AND algebraic_expression %prec AND
                  {
                    using LogicBinOp = BinOpNode<ast::LogicOpType>;
                    auto expr = driver->make_node<LogicBinOp>($1, $3, ast::LogicOpType::AND);
                    $$ = driver->make_node<LogicExprNode>(std::move(expr), ast::LogicOpType::AND);       
                  }     
                | algebraic_expression OR algebraic_expression %prec OR
                  {
                    using LogicBinOp = BinOpNode<ast::LogicOpType>;
                    auto expr = driver->make_node<LogicBinOp>($1, $3, ast::LogicOpType::OR);
                    $$ = driver->make_node<LogicExprNode>(std::move(expr), ast::LogicOpType::OR);       
                  }
                | NOT algebraic_expression %prec NOT 
                  {
                    $$ = driver->make_node<LogicExprNode>($2, ast::LogicOpType::NOT); 
                  }
;

subexpr: terminal                  { $$ = $1; }
       | LPAREN expression RPAREN  { $$ = $2; }
       | input                     { $$ = $1; }
;

terminal: number    { $$ = $1; }
        | variable  { 
                      $$ = $1;
                      if(!driver->is_variable_declared($1->get_id())) 
                         parser::error(@$, "'" + $1->get_id() + 
                                       "' was not declared in this scope"); 
                    }
;

number: NUMBER { $$ = driver->make_node<NumberNode>($1); }
;

variable: ID  { $$ = driver->make_or_assign<VariableNode>($1); } 
;

%%

namespace yy 
{
     parser::token_type yylex( parser::semantic_type* yylval,
                               parser::location_type* yylloc,                         
                               Driver* driver )
     {
          *yylloc = driver->get_current_location();
          return driver->yylex(yylloc, yylval);
     }

     void parser::error(const parser::location_type& loc, const std::string& errorMessage)
     { 
          driver->set_executable_status(false);
          const int line = driver->get_current_line();
          const int column = driver->get_current_column();
          std::string msg = errorreport::prepare_error_message(errorMessage);
          std::cerr << line << ":" << column << ": " << msg << std::endl;
     }
}  //  namespace yy