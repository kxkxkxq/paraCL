#pragma once

#include <iostream>
#include <vector>

#include <FlexLexer.h>

#include "node.hpp"
#include "pcl_grammar.tab.hh"

namespace yy
{
    class Driver final
    {
        FlexLexer *pLexer_;
        CurrentScopeNode* ast_;

    public :
        Driver(FlexLexer *l) : pLexer_(l) {};
        
        parser::token_type yylex(parser::semantic_type *yyval)
        {
            parser::token_type tokenType = static_cast<parser::token_type>(pLexer_->yylex());
            
            if (tokenType == yy::parser::token_type::NUMBER)
                yyval->as<int>() = std::stoi(pLexer_->YYText());   
            return tokenType;
        }

        bool parse()
        {
            parser parser(this);
            bool res = parser.parse();
            return !res;
        }

        void execute()
        {
            for(auto&& stmnt : ast_)
                stmnt.execute();
        }

#if 0  //  will be implemented later
        void print_ast() {....}
#endif    
        void insert_current_scope(CurrentScopeNode* curScope)
        {
            ast_ = std::exchange(curScope, nullptr);
        }
    };
}   //  namespace yy