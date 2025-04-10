%option c++
%option yylineno
%option yyclass="yy::Lexer"

%{
    
#include "lexer.hpp"
#define YY_USER_ACTION update_current_location();

%}

WS      [ \t\r\v]+
NL      "\n"
DIGIT   [0-9]
DIGIT1  [1-9]
OP      [\+\-\=\*\/\<\>\%]
SEP     [\{\}\(/)]
SCOLON  ";"+
NUMBER  {DIGIT1}{DIGIT}*|0
ID      [a-zA-Z_][a-zA-Z_0-9]*

%%

{NL}                                
{WS}                                    //  skip
{OP}|"<="|">="|"!="|"=="|"&&"|"||"|"!"  { return yy::process_operator(yytext); }
{NUMBER}                                { return yy::process_number(yytext); }
"print"|"if"|"else"|"while"|"?"         { return yy::process_keyword(yytext); }
{ID}                                    { return yy::process_identifier(yytext); }
{SEP}                                   { return yy::process_separator(yytext); }
{SCOLON}                                { return yy::process_scolon(yytext); }
.                                       {
                                          int l = get_current_line();
                                          int c = get_current_column();
                                          std::string errM = "lexical error, stray token ";
                                          std::cerr << l << ":" << c << ":" << errM + "'" << YYText() << "'" << std::endl; 
                                        }

%%  //  nothing