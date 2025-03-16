//-------------------------------------------------------------------------------------------------
//
//  Grammar for paraCL parser :
//                 statements -> statement; statements | empty 
//                  statement -> expression
//                 expression -> assignment | additive_expression
//                 assignment -> variable | expression
//        additive_expression -> additive_expression bin_add_oper multiplicative_expression
//                               | multiplicative_expression
//  multiplicative_expression -> multiplicative_expression bin_mul_oper terminal | terminal
//                   terminal -> number | variable | ( expression )
//                   variable -> id 
//
//-------------------------------------------------------------------------------------------------

%language "c++"

%skeleton "lalr1.cc"
%defines
%define api.value.type variant
%param {yy::Driver* driver}

%code 
{
    #include "driver.hpp"

    namespace yy
    {
        parser::token_type yylex( parser::semantic_type* yylval,                         
                                  Driver* driver );
    }
}

%token
    ASSIGN  "="
    MINUS   "-"
    PLUS    "+"
    DIV     "/"
    MUL     "*"
    LESS    "<"
    GREATER ">"
    SCOLON  ";"
    LCBR    "{"
    RCBR    "}"
    LPAREN  "("
    RPAREN  ")"
    ERR     
;
%token <int> NUMBER
%token <std::string> ID
%nterm statements statement expression assignment additive_expression multiplicative_expression

%left '-' '+'
%left '/' '*'


%start program
%%

program: exprlist
;

exprlist: expr SCOLON exprlist
        | %empty
;

expr: id EQUAL e    {
                        $$ = // doing smth 
                    }
;

e: t            { $$ = // doing smth }
 | e MINUS t    { $$ = // doing smth }
 | e PLUS t     { $$ = // doing smth }
;

t: f           { $$ = // doing smth }
 | t DIV f     { $$ = // doing smth }
 | t MUL f     { $$ = // doing smth }
;

f: num                  { $$ = // doing smth }
 | id                   { $$ = // doing smth }
 | LPAREN e RPAREN      { $$ = // doing smth }
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