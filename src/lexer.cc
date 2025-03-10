#include <iostream>
#include <memory>

#include "lexer.hpp"

int yyFlexLexer::yywrap() { return 1; }

int main() {
  auto lexer = std::make_unique<Lexer>();
  while (lexer->yylex() != 0) {
    #define DBG_LEXER
    #ifdef DBG_LEXER
    lexer->print_lexem();
    #endif // DBG_LEXER
  }

  return 0;
}
