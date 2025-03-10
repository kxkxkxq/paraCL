%option yyclass="Lexer"
%option c++

%{

#include "lexer.hpp"

%}

WS      [ \t\n\r\v]+
DIGIT   [0-9]
DIGIT1  [1-9]
OP      [\+\-\=\*\/\?\<\>]
SEP     [;{}()]
ID      [a-zA-Z_][a-zA-Z_0-9]*

%%

{WS}                            /* skip */

{OP}|"<="|">="|"!="|"||"|"&&"   return process_operator();

{DIGIT1}{DIGIT}*|{DIGIT}        return process_number();

"print"|"if"|"else"|"while"     return process_keyword();

{ID}                            return process_identifier();

{SEP}                           return process_separator();

.                               return process_unknown();

%%
