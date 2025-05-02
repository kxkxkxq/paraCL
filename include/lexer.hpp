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

    const int get_current_line() const noexcept 
    { 
      return currentLocation_.end.line; 
    }

    const int get_current_column() const noexcept 
    { 
      return currentLocation_.end.column; 
    }

    int yylex();
  };
}  // namespace yy