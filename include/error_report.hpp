#pragma once

#include <string>
#include <sstream>

namespace errorreport
{
    inline std::string prepare_error_message(const std::string msg)
    {
        std::basic_istringstream input(msg);
        std::string result;
        std::string buffer;
        
        while(input >> buffer)
        {
                 if (buffer == "PRINT"   || buffer == "PRINT,")   { buffer = "keyword 'print',"; }
            else if (buffer == "IF"      || buffer == "IF,")      { buffer = "keyword 'if',"; }
            else if (buffer == "ELSE"    || buffer == "ELSE,")    { buffer = "keyword 'else',"; }
            else if (buffer == "WHILE"   || buffer == "WHILE,")   { buffer = "keyword 'while',"; }
            else if (buffer == "INPUT"   || buffer == "INPUT,")   { buffer = "keyword '?',"; }
            else if (buffer == "ID"      || buffer == "ID,")      { buffer = "identifier,"; }
            else if (buffer == "NUMBER"  || buffer == "NUMBER,")  { buffer = "integer number,"; }
            else if (buffer == "LEQUAL"  || buffer == "LEQUAL,")  { buffer = "'<=',"; }
            else if (buffer == "GEQUAL"  || buffer == "GEQUAL,")  { buffer = "'>=',"; }
            else if (buffer == "NEQUAL"  || buffer == "NEQUAL,")  { buffer = "'!=',"; }
            else if (buffer == "EQUAL"   || buffer == "EQUAL,")   { buffer = "'==',"; }
            else if (buffer == "AND"     || buffer == "AND,")     { buffer = "'&&',"; }
            else if (buffer == "OR"      || buffer == "OR,")      { buffer = "'||',"; }
            else if (buffer == "SCOLON"  || buffer == "SCOLON,")  { buffer = "';',"; }
            else if (buffer == "LCBR"    || buffer == "LCBR,")    { buffer = "'{',"; }
            else if (buffer == "RCBR"    || buffer == "RCBR,")    { buffer = "'}',"; }
            else if (buffer == "LPAREN"  || buffer == "LPAREN,")  { buffer = "'(',"; }
            else if (buffer == "RPAREN"  || buffer == "RPAREN,")  { buffer = "')',"; }
            else if (buffer == "PLUS"    || buffer == "PLUS," )   { buffer = "'+',"; }
            else if (buffer == "MINUS"   || buffer == "MINUS,")   { buffer = "'-',"; }
            else if (buffer == "ASSIGN"  || buffer == "ASSIGN,")  { buffer = "'=',"; }
            else if (buffer == "MUL"     || buffer == "MUL,")     { buffer = "'*',"; }
            else if (buffer == "DIV"     || buffer == "DIV,")     { buffer = "'/',"; }
            else if (buffer == "LESS"    || buffer == "LESS,")    { buffer = "'<',"; }
            else if (buffer == "GREATER" || buffer == "GREATER,") { buffer = "'>',"; }
            else if (buffer == "MOD"     || buffer == "MOD,")     { buffer = "'%',"; }
            else if (buffer == "NOT"     || buffer == "NOT,")     { buffer = "'!',"; }
            result += (buffer + " ");
        }
        result.pop_back();  //  to delete excess ' '
        if(result.back() == ',') result.pop_back();  //  to delete excess ','
        return result;
    }
}