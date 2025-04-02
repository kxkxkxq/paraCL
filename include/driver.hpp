//-------------------------------------------------------------------------------------------------
//
//  Driver implementation
//
//-------------------------------------------------------------------------------------------------
#pragma once

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>
#include <stack>
#include <type_traits>
#include <utility>
#include <vector>
#include <FlexLexer.h>

#include "node.hpp"
#include "lexer.hpp"
#include "ast_builder.hpp"
#include "pcl_grammar.tab.hh"

namespace yy
{
    class Driver final
    {
        Lexer lexer_;
        ast::Builder astBuilder_;
        ast::CurrentScopeNode* ast_ = nullptr;
        std::vector<CurrentScopeNode*> scopeStorage; 

    public :
        Driver() = default;
        
        parser::token_type yylex(parser::location_type* yylloc, parser::semantic_type* yyval)
        {
            parser::token_type tokenType = static_cast<parser::token_type>(lexer_.yylex());

            if (tokenType == yy::parser::token_type::NUMBER)
            {
                yyval->as<int>() = std::stoi(lexer_.YYText());   
                return tokenType;
            }
            if (tokenType == yy::parser::token_type::ID)
            {
                yyval->emplace<std::string>(lexer_.YYText());
                return tokenType; 
            }
            return tokenType;
        }

        void set_input_stream(std::istream& inputStream)
        {
            assert(inputStream);
            lexer_.switch_streams(&inputStream, &std::cout);
        }

        bool parse()
        {
            parser parser(this);
            bool res = parser.parse();
            return !res;
        }

        parser::location_type& get_current_location() { return lexer_.get_current_location(); }

        void set_ast_root(CurrentScopeNode* curScope)
        {
            assert(curScope);
            ast_ = curScope;
        }

        CurrentScopeNode* get_current_scope() { assert(scopeStorage.back()); return scopeStorage.back(); }

        template <typename NodeType, class... Args>
        NodeType* make_node(Args&&... args) { return astBuilder_.make_node<NodeType>(args ...); }

        void descend_into_scope(CurrentScopeNode* currScope)
        {
            std::cerr << "descend_into_scope()" << std::endl;
            std::cerr << "!!  DEBUG INFO : " << std::endl;
            if(!scopeStorage.empty())
                std::cerr << "previous scope : " << scopeStorage.back() << std::endl;
            std::cerr << "current scope : " << currScope << std::endl;
            assert(currScope);
            scopeStorage.emplace_back(currScope);
            assert(currScope == scopeStorage.back());
        }

        void ascend_from_scope()
        {
            std::cerr << "ascend_from_scope()" << std::endl;
            assert(!scopeStorage.empty());
            scopeStorage.pop_back();
        }

        void add_to_context(VariableNode* var)
        {
            assert(var);
            scopeStorage.back()->add_to_context(var);
        }
        
        VariableNode* find_variable(const std::string id) const
        {
            auto is_declared = [&id] (CurrentScopeNode* sc) { return sc->is_declared(id); };
            auto iter = std::find_if(scopeStorage.rbegin(), scopeStorage.rend(), is_declared);
            
            if(iter == scopeStorage.rend())
                return nullptr;
            return (*iter)->get_variable(id);
        }

        bool is_variable_declared(const std::string id) const { return find_variable(id) != nullptr; }

        template <typename NodeType, class... Args>
        NodeType* make_or_assign(Args&&... args)
        {
            if(std::is_same_v<NodeType, ast::VariableNode>)
            {
                if(is_variable_declared(args ...))   
                    return find_variable(args ...);
            }
            return make_node<NodeType>(args ...);
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