%option c++
%option yylineno
%option yyclass="yy::Lexer"

%{
    
#include "lexer.hpp"
#define YY_USER_ACTION update_current_location();

%}

COMMENT  "//".* 
WS       [ \t\r\v]+
NL       "\n"
DIGIT    [0-9]
DIGIT1   [1-9]
SCOLON   ";"+
NUMBER   {DIGIT1}{DIGIT}*|0
ID       [a-zA-Z_][a-zA-Z_0-9]*

%%

{COMMENT}  // skip  
{WS}       // skip
{NL}       // skip 

"+"        { return yy::parser::token_type::PLUS; }
"-"        { return yy::parser::token_type::MINUS; }
"*"        { return yy::parser::token_type::MUL; }
"/"        { return yy::parser::token_type::DIV; }
"%"        { return yy::parser::token_type::MOD; }
"="        { return yy::parser::token_type::ASSIGN; }
"<"        { return yy::parser::token_type::LESS; }
">"        { return yy::parser::token_type::GREATER; }
"<="       { return yy::parser::token_type::LEQUAL; }
">="       { return yy::parser::token_type::GEQUAL; }
"!="       { return yy::parser::token_type::NEQUAL; }
"=="       { return yy::parser::token_type::EQUAL; } 
"&&"       { return yy::parser::token_type::AND; }
"||"       { return yy::parser::token_type::OR; }
"!"        { return yy::parser::token_type::NOT; }

"print"    { return yy::parser::token_type::PRINT; }
"if"       { return yy::parser::token_type::IF; }
"else"     { return yy::parser::token_type::ELSE; }
"while"    { return yy::parser::token_type::WHILE; }
"?"        { return yy::parser::token_type::INPUT; } 

{NUMBER}   { return yy::parser::token_type::NUMBER; }
{ID}       { return yy::parser::token_type::ID; }

"{"        { return yy::parser::token_type::LCBR; }
"}"        { return yy::parser::token_type::RCBR; }
"("        { return yy::parser::token_type::LPAREN; }
")"        { return yy::parser::token_type::RPAREN; }
{SCOLON}   { return yy::parser::token_type::SCOLON; }
.          {
             int l = get_current_line();
             int c = get_current_column();
             std::string errM = "lexical error, stray token ";
             std::cerr << l << ":" << c << ":" << errM + "'" 
                       << YYText() << "'" << std::endl;
             return yy::parser::token_type::ERROR; 
           }
%%  // nothing