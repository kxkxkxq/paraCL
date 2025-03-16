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
    
         if (curr_value == "+") return yy::parser::token_type::PLUS;
    else if (curr_value == "-") return yy::parser::token_type::MINUS;
    else if (curr_value == "=") return yy::parser::token_type::ASSIGN;
    else if (curr_value == "*") return yy::parser::token_type::MUL;
    else if (curr_value == "/") return yy::parser::token_type::DIV;
    else if (curr_value == "<") return yy::parser::token_type::LESS;
    else if (curr_value == ">") return yy::parser::token_type::GREATER;
  //  case "<=" : return yy::parser::token_type:: ....
  //  case ">=" : return yy::parser::token_type:: ....
  //  case "!=" : return yy::parser::token_type:: ....
  //  case "==" : return yy::parser::token_type:: ....
    else return yy::parser::token_type::ERROR;
  }

  int process_number() {
    curr_lexem = "number";
    curr_value = yytext;
    return yy::parser::token_type::NUMBER;
  }

  int process_keyword() {
    curr_lexem = "keyword";
    curr_value = yytext;
    
         if (curr_value == "print") return yy::parser::token_type::OUTPUT;
    else if (curr_value == "if")    return yy::parser::token_type::IF;
    else if (curr_value == "while") return yy::parser::token_type::WHILE;
    else return yy::parser::token_type::ERROR;
  }

  int process_identifier() {
    curr_lexem = "identifier";
    curr_value = yytext;
    return yy::parser::token_type::ID;
  }

  int process_separator() {
    curr_lexem = "separator";
    curr_value = yytext[0];
         if (curr_value == ";") return yy::parser::token_type::SCOLON;
    else if (curr_value == "{") return yy::parser::token_type::LCBR;
    else if (curr_value == "}") return yy::parser::token_type::RCBR;
    else if (curr_value == "(") return yy::parser::token_type::LPAREN;
    else if (curr_value == ")") return yy::parser::token_type::RPAREN;
    else return yy::parser::token_type::ERROR;
  }

  int process_unknown() {
    curr_lexem = "unknown";
    curr_value = yytext[0];
    return yy::parser::token_type::ERROR; 
  }

public:
  int yylex() override;

  void print_lexem() const {
    std::cout << ">> lexer read " << curr_lexem << " [" << curr_value << "]\n";
  }

};

#endif // LEXER_HPP
