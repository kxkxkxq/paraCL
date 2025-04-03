//-------------------------------------------------------------------------------------------------
//
//  AST builder - auxiliary class for sequential tree building 
//  by creating nodes and storing unique pointers to them in the buffer
//
//-------------------------------------------------------------------------------------------------
#pragma once

#include <cassert>
#include <vector>
#include <memory>

#include "node.hpp" 

namespace ast
{
    class Builder final
    {
        std::vector<std::unique_ptr<INode>> astBuffer_;

    public :
        template <typename NodeType, class... Args>
        NodeType* make_node(Args&&... args)
        {
            astBuffer_.emplace_back(std::make_unique<NodeType>(args ...));
            assert(astBuffer_.back());
            return static_cast<NodeType*>((astBuffer_.back()).get());
        }
    };
}  // namespace ast