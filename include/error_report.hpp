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
                 if (buffer == "ERROR"   || buffer == "ERROR,")   { buffer = "stray token in program,"; }
            else if (buffer == "SCOLON"  || buffer == "SCOLON,")  { buffer = "';',"; }
            else if (buffer == "LCBR"    || buffer == "LCBR,")    { buffer = "'(',"; }
            else if (buffer == "RCBR"    || buffer == "RCBR,")    { buffer = "')',"; }
            else if (buffer == "LPAREN"  || buffer == "LPAREN,")  { buffer = "'{',"; }
            else if (buffer == "RPAREN"  || buffer == "RPAREN,")  { buffer = "'}',"; }
            else if (buffer == "PRINT"   || buffer == "PRINT,")   { buffer = "'print',"; }
            else if (buffer == "IF"      || buffer == "IF,")      { buffer = "'if',"; }
            else if (buffer == "WHILE"   || buffer == "WHILE,")   { buffer = "'while',"; }
            else if (buffer == "INPUT"   || buffer == "INPUT,")   { buffer = "'?',"; }
            else if (buffer == "ID"      || buffer == "ID,")      { buffer = "'id',"; }
            else if (buffer == "NUMBER"  || buffer == "NUMBER,")  { buffer = "'number',"; }
            else if (buffer == "PLUS"    || buffer == "PLUS," )   { buffer = "'+',"; }
            else if (buffer == "MINUS"   || buffer == "MINUS,")   { buffer = "'-',"; }
            else if (buffer == "ASSIGN"  || buffer == "ASSIGN,")  { buffer = "'=',"; }
            else if (buffer == "MUL"     || buffer == "MUL,")     { buffer = "'*',"; }
            else if (buffer == "DIV"     || buffer == "DIV,")     { buffer = "'/',"; }
            else if (buffer == "LESS"    || buffer == "LESS,")    { buffer = "'<',"; }
            else if (buffer == "GREATER" || buffer == "GREATER,") { buffer = "'>',"; }
            else if (buffer == "MOD"     || buffer == "MOD,")     { buffer = "'%',"; }
            else if (buffer == "LEQUAL"  || buffer == "LEQUAL,")  { buffer = "'<=',"; }
            else if (buffer == "GEQUAL"  || buffer == "GEQUAL,")  { buffer = "'>=',"; }
            else if (buffer == "NEQUAL"  || buffer == "NEQUAL,")  { buffer = "'!=',"; }
            else if (buffer == "EQUAL"   || buffer == "EQUAL,")   { buffer = "'==',"; }
            else if (buffer == "AND"     || buffer == "AND,")     { buffer = "'&&',"; }
            else if (buffer == "OR"      || buffer == "OR,")      { buffer = "'||',"; }
            result += (buffer + " ");
        }
        result.pop_back();  //  to delete excess ' '
        if(result.back() == ',') result.pop_back();  //  to delete excess ','
        return result;
    }
}