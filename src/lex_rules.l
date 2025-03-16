%option yyclass="Lexer"
%option c++

%{

#include "lexer.hpp"
#include "pcl_grammar.tab.hh"

%}

WS      [ \t\n\r\v]+
DIGIT   [0-9]
DIGIT1  [1-9]
OP      [\+\-\=\*\/\<\>]
SEP     [\;\{\}\(/)]
ID      [a-zA-Z_][a-zA-Z_0-9]*

%%

{WS}                            /* skip */

{OP} | "<=" | ">=" | "!=" | "=="  { return process_operator(); }

"?"                               { return process_input(); }

{DIGIT1}{DIGIT}* | {DIGIT}        { return process_number(); }

"print" | "if" | "while"          { return process_keyword(); }

{ID}                              { return process_identifier(); }

{SEP}                             { return process_separator(); }

.                                 { return process_unknown(); }

%%
