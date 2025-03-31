//-------------------------------------------------------------------------------------------------
//
//  Auxiliary functions for processing incoming lexemes
//
//-------------------------------------------------------------------------------------------------
#pragma once

#include <iostream>
#include <string>

#include "pcl_grammar.tab.hh"

namespace yyproxy
{

  void print_lexem(std::string l, std::string v) {
    std::cout << ">> lexer read " << l << " [" << v << "]\n";
  }

  int process_operator(const std::string& text) {
    std::string curr_lexem = "operator";
    std::string curr_value = text;
    print_lexem(curr_lexem, curr_value);
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
    else return yy::parser::token_type::ERROR;
  }

  int process_number(const std::string& text) {
    std::string curr_lexem = "number";
    std::string curr_value = text;
    print_lexem(curr_lexem, curr_value);
    return yy::parser::token_type::NUMBER;
  }

  int process_keyword(const std::string& text) {
    std::string curr_lexem = "keyword";
    std::string curr_value = text;
    print_lexem(curr_lexem, curr_value);
         if (curr_value == "print") return yy::parser::token_type::PRINT;
    else if (curr_value == "if")    return yy::parser::token_type::IF;
    else if (curr_value == "while") return yy::parser::token_type::WHILE;
    else if (curr_value == "?")     return yy::parser::token_type::INPUT;
    else return yy::parser::token_type::ERROR;
  }

  int process_identifier(const std::string& text) {
    std::string curr_lexem = "identifier";
    std::string curr_value = text;
    print_lexem(curr_lexem, curr_value);
    return yy::parser::token_type::ID;
  }

  int process_separator(const std::string& text) {
    std::string curr_lexem = "separator";
    std::string curr_value = text;
    print_lexem(curr_lexem, curr_value);
         if (curr_value == "{") return yy::parser::token_type::LCBR;
    else if (curr_value == "}") return yy::parser::token_type::RCBR;
    else if (curr_value == "(") return yy::parser::token_type::LPAREN;
    else if (curr_value == ")") return yy::parser::token_type::RPAREN;
    else return yy::parser::token_type::ERROR;
  }

  int process_scolon(const std::string& text) {
    std::string curr_lexem = "scolon";
    std::string curr_value = text;
    print_lexem(curr_lexem, curr_value);
    return yy::parser::token_type::SCOLON;
  }

  int process_unknown(const std::string& text) {
    std::string curr_lexem = "unknown";
    std::string curr_value = text;
    print_lexem(curr_lexem, curr_value);
    return yy::parser::token_type::ERROR; 
  }
}  //  namespace yyproxy
