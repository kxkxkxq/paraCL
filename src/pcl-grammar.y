//-----------------------------------------------------------------------------
//
//  Grammar for paraCL parser :
//      EXPRL  ->   EXPR; EXPRL | empty
//      EXPR   ->   VAR = E
//      E      ->   T | E - T | E + T   //  need to rename : T, E, F, ...
//      T      ->   F | T / F | T * F   
//      F      ->   NUM | VAR | ( E )   
//      VAR    ->   ID
//    
//  now it is a simple grammar without scopes, conditional statements and loops
//  and keywords
//-----------------------------------------------------------------------------

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
    EQUAL   "="
    MINUS   "-"
    PLUS    "+"
    DIV     "/"
    MUL     "*"
    SCOLON  ";"
    LPAREN  "("
    RPAREN  ")"
    ERR     
;
%token <int>         num
%token <std::string> id
%nterm expr e t f   //  need to rename

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