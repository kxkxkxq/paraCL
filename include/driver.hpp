#pragma once

#include <vector>
#include <FlexLexer.h>

#include "node.hpp"
#include "pcl_grammar.tab.hh"

namespace yy
{
    class Driver final
    {
        FlexLexer *pLexer_;
        std::vector<statement_node> statements_;

    public :
        Driver(FlexLexer *l) : pLexer_(l) {};
        
        parser::token_type yylex(parser::semantic_type *yyval)
        {
            parser::token_type tokenType = static_cast<parser::token_type>(pLexer_->yylex());
            
            if (tokenType == yy:parser::token_type::NUMBER)
                yyval->as<int>() = std::stoi(pLexer_->YYText());
            if (tokenType == yy::parser::token_type::ID)
                *yyval = static_cast<std::string>(pLexer_->YYText());

            return tokenType;
        }

        bool parse()
        {
            parser parser(this);
            bool res = parser.parse();
            return !res;
        }

        void insert_statements(const std::vector<statement_node> st)
        {
            statements_.assign(st.begin(), st.end());
        }
    };
}   //  namespace yy