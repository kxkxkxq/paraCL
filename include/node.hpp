#pragma once

#include <>

namespace astnodes
{
    enum class NodeType
    {

    };
    
    enum class BinOpNodeType
    {

    };

    enum class StNodeType
    {

    };

    enum class ExprNodeType
    {

    };
    
    struct Node
    {
        Node *parent = nullptr;
        NodeType nType;
        virtual ~Node() {}
    };

    struct StatementNode 
    {
        StNodeType stType;
        virtual ~StatementNode() {}
    };

    struct BinOpNode final : public Node
    {
        BinOpNodeType opType;
        ExpressionNode *lChild = nullptr;
        ExpressionNode *rChild = nullptr;

    public:
        BinOpNode(Node *parent, BinOpNodeType opcode) : Node{parent, nType::BINOP}, opType(opcode) {} 

        void push_left_child ()
        {

        }
    };

    struct ExpressionNode : public StatementNode
    {

    };

    struct AssignmentNode : public ExpressionNode
    {

    };

    struct ArithmeticNode : public ExpressionNode
    {
        
    };











}   //  namespace astnodes