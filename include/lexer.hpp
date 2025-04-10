#pragma once

#include <iostream>
#include <string>

#include "pcl_grammar.tab.hh"

namespace yy
{
  class Lexer final : public yyFlexLexer
  {
      parser::location_type currentLocation_;
  
  public:
      void update_current_location()
      {
        std::string lexem = YYText();
        if (lexem == "\n")
        {
          currentLocation_.end.column = 0;
          currentLocation_.end.line = yylineno;
        }
        else
          currentLocation_.end.column += yyleng;
        currentLocation_.step();
      }

      parser::location_type& get_current_location()
      {
        return currentLocation_;
      }

      const int get_current_line() const noexcept { return currentLocation_.end.line; }
      const int get_current_column() const noexcept { return currentLocation_.end.column; }

      int yylex();
  };
  
//  AUXILIARY FUNCTIONS FOR PROCESSING INCOMING LEXEMES  
  inline void print_lexem(std::string l, std::string v) {
    std::cout << ">> lexer read " << l << " [" << v << "]\n";
  }

  inline int process_operator(const std::string& text) {
    std::string curr_lexem = "operator";
    std::string curr_value = text;
         if (curr_value == "+")  return yy::parser::token_type::PLUS;
    else if (curr_value == "-")  return yy::parser::token_type::MINUS;
    else if (curr_value == "=")  return yy::parser::token_type::ASSIGN;
    else if (curr_value == "*")  return yy::parser::token_type::MUL;
    else if (curr_value == "/")  return yy::parser::token_type::DIV;
    else if (curr_value == "<")  return yy::parser::token_type::LESS;
    else if (curr_value == ">")  return yy::parser::token_type::GREATER;
    else if (curr_value == "%")  return yy::parser::token_type::MOD;
    else if (curr_value == "<=") return yy::parser::token_type::LEQUAL; 
    else if (curr_value == ">=") return yy::parser::token_type::GEQUAL;
    else if (curr_value == "!=") return yy::parser::token_type::NEQUAL;
    else if (curr_value == "==") return yy::parser::token_type::EQUAL;
    else if (curr_value == "&&") return yy::parser::token_type::AND;
    else if (curr_value == "||") return yy::parser::token_type::OR;
    else if (curr_value == "!")  return yy::parser::token_type::NOT;
    else return yy::parser::token_type::ERROR;
  }

  inline int process_number(const std::string& text) {
    std::string curr_lexem = "number";
    std::string curr_value = text;
    return yy::parser::token_type::NUMBER;
  }

  inline int process_keyword(const std::string& text) {
    std::string curr_lexem = "keyword";
    std::string curr_value = text;
         if (curr_value == "print") return yy::parser::token_type::PRINT;
    else if (curr_value == "if")    return yy::parser::token_type::IF;
    else if (curr_value == "else")  return yy::parser::token_type::ELSE;
    else if (curr_value == "while") return yy::parser::token_type::WHILE;
    else if (curr_value == "?")     return yy::parser::token_type::INPUT;
    else return yy::parser::token_type::ERROR;
  }

  inline int process_identifier(const std::string& text) {
    std::string curr_lexem = "identifier";
    std::string curr_value = text;
    return yy::parser::token_type::ID;
  }

  inline int process_separator(const std::string& text) {
    std::string curr_lexem = "separator";
    std::string curr_value = text;
         if (curr_value == "{") return yy::parser::token_type::LCBR;
    else if (curr_value == "}") return yy::parser::token_type::RCBR;
    else if (curr_value == "(") return yy::parser::token_type::LPAREN;
    else if (curr_value == ")") return yy::parser::token_type::RPAREN;
    else return yy::parser::token_type::ERROR;
  }

  inline int process_scolon(const std::string& text) {
    std::string curr_lexem = "scolon";
    std::string curr_value = text;
    return yy::parser::token_type::SCOLON;
  }

  inline int process_unknown(const std::string& text) {
    std::string curr_lexem = "unknown";
    std::string curr_value = text;
    return yy::parser::token_type::ERROR; 
  }
}  //  namespace yy