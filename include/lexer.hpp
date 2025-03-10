#ifndef LEXER_HPP
#define LEXER_HPP

#include <iostream>
#include <string>

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif // yyFlexLexer

class Lexer : public yyFlexLexer {
  std::string curr_lexem;
  std::string curr_value;

  int process_operator() {
    curr_lexem = "operator";
    curr_value = yytext;
    return 1;
  }

  int process_number() {
    curr_lexem = "number";
    curr_value = yytext;
    return 1;
  }

  int process_keyword() {
    curr_lexem = "keyword";
    curr_value = yytext;
    return 1;
  }

  int process_identifier() {
    curr_lexem = "identifier";
    curr_value = yytext;
    return 1;
  }

  int process_separator() {
    curr_lexem = "separator";
    curr_value = yytext;
    return 1;
  }

  int process_unknown() {
    curr_lexem = "unknown";
    curr_value = yytext[0];
    return 1;
  }

public:
  int yylex() override;

  void print_lexem() const {
    std::cout << curr_lexem << " [" << curr_value << "]\n";
  }

};

#endif // LEXER_HPP
