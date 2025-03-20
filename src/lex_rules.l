%option c++

%{

#include "lexer.hpp"

%}

WS      [ \t\n\r\v]+
DIGIT   [0-9]
DIGIT1  [1-9]
OP      [\+\-\=\*\/\<\>]
SEP     [\;\{\}\(/)]
NUMBER  {DIGIT1}{DIGIT}*|0
ID      [a-zA-Z_][a-zA-Z_0-9]*

%%

{WS}                              //  skip
{OP}|"<="|">="|"!="|"=="          { return yyproxy::process_operator(yytext); }
{NUMBER}                          { return yyproxy::process_number(yytext); }
"print"|"if"|"while"|"?"          { return yyproxy::process_keyword(yytext); }
{ID}                              { return yyproxy::process_identifier(yytext); }
{SEP}                             { return yyproxy::process_separator(yytext); }
.                                 { return yyproxy::process_unknown(yytext); }

%%
