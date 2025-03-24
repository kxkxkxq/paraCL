//-------------------------------------------------------------------------------------------------
//
//  Driver implementation
//
//-------------------------------------------------------------------------------------------------
#pragma once

#include <iostream>
#include <vector>
#include <utility>

#include <FlexLexer.h>

#include "node.hpp"
#include "ast_builder.hpp"
#include "pcl_grammar.tab.hh"

namespace yy
{
    class Driver final
    {
        FlexLexer *pLexer_;
        ast::Builder astBuilder_;
        ast::CurrentScopeNode* ast_;

    public :
        Driver(FlexLexer *l) : pLexer_(l) {};
        
        parser::token_type yylex(parser::semantic_type *yyval)
        {
            parser::token_type tokenType = static_cast<parser::token_type>(pLexer_->yylex());
            
            if (tokenType == yy::parser::token_type::NUMBER)
            {
                yyval->as<int>() = std::stoi(pLexer_->YYText());   
                return tokenType;
            }
            if (tokenType == yy::parser::token_type::ID)
            {
                yyval->emplace<std::string>(pLexer_->YYText());
                return tokenType; 
            }
            return tokenType;
        }

        bool parse()
        {
            parser parser(this);
            bool res = parser.parse();
            return !res;
        }

        void set_ast_root(CurrentScopeNode* curScope)
        {
            assert(curScope);
            ast_ = curScope;
        }

        template <typename NodeType, class... Args>
        NodeType* make_node(Args&&... args)
        {
            return astBuilder_.make_node<NodeType>(args ...);
        }

        void execute()
        {
            assert(ast_);
            ast_->execute();
        }

#if 0  //  will be implemented later
        void print_ast() {....}
#endif    
    };
}   //  namespace yy