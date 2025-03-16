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
    
    if (curr_value == "+") : return yylex::parser::token_type::PLUS;
    if (curr_value == "-") : return yylex::parser::token_type::MINUS;
    if (curr_value == "=") : return yylex::parser::token_type::ASSIGN;
    if (curr_value == "*") : return yylex::parser::token_type::MUL;
    if (curr_value == "/") : return yylex::parser::token_type::DIV;
    if (curr_value == "<") : return yylex::parser::token_type::LESS;
    if (curr_value == ">") : return yylex::parser::token_type::GREATER;
  //  case "<=" : return yylex::parser::token_type:: ....
  //  case ">=" : return yylex::parser::token_type:: ....
  //  case "!=" : return yylex::parser::token_type:: ....
  //  case "==" : return yylex::parser::token_type:: ....
  //  return unknown ....
  }

  int process_number() {
    curr_lexem = "number";
    curr_value = yytext;
    return yylex::parser::token_type::NUMBER;
  }

  int process_keyword() {
    curr_lexem = "keyword";
    curr_value = yytext;
    
    if (curr_value == "print") : return yylex::parser::token_type::OUTPUT;
    if (curr_value == "if")    : return yylex::parser::token_type::IF;
    if (curr_value == "while") : return yylex::parser::token_type::WHILE;
  //  return unknown ....
  }

  int process_identifier() {
    curr_lexem = "identifier";
    curr_value = yytext;
    return yylex::parser::token_type::ID;
  }

  int process_separator() {
    curr_lexem = "separator";
    curr_value = yytext[0];
    if (curr_value == ";") : return yylex::parser::token_type::SCOLON;
    if (curr_value == "{") : return yylex::parser::token_type::LCBR;
    if (curr_value == "}") : return yylex::parser::token_type::RCBR;
    if (curr_value == "(") : return yylex::parser::token_type::LPAREN;
    if (curr_value == ")") : return yylex::parser::token_type::RPAREN;
  }

  int process_unknown() {
    curr_lexem = "unknown";
    curr_value = yytext[0];
  //  return anknown ....
  }

public:
  int yylex() override;

  void print_lexem() const {
    std::cout << curr_lexem << " [" << curr_value << "]\n";
  }

};

#endif // LEXER_HPP
