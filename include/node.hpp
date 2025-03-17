#pragma once

#include <vector>
#include <string>

namespace astnodes
{
    enum class NodeType
    {
        BINOP,
        ID,
        NUMBER,
        STATEMENT,
        SCOPE
    };
    
    enum class BinOpType
    {
        ASSIGN,
        MINUS,
        PLUS,
        DIV,
        MUL,
        LESS,
        GREATER,
        EQUAL
    };

    enum class StatementType
    {
        EXPRESSION,
        IF,
        WHILE
    };

    struct INode
    {
        INode* parent = nullptr;
        NodeType nType;
        virtual ~INode() {}
    };

    class IdNode : public INode
    {
        std::string id_;
        int value_;

    public :
        IdNode(INode* par, std::string i, int v) : INode{par, NodeType::ID}, id_(i), value_(v) {}

        std::string get_id() { return id_; }

        int get_value() { return value_; }
    };

    class NumberNode : public INode
    {
        int constant_;

    public : 
        NumberNode(INode* par, int c) : INode{par, NodeType::CONSTANT}, constant_(c) {}

        int get_constant() { return constant_; }  
    };

    class ScopeNode : public INode
    {
        std::vector<INode*> scopeVec_;

    public : 
        ScopeNode(INode* par) : INode{par, NodeType::SCOPE} {}

        const std::vector<INode*> get_scopeVec() { return scopeVec_; } 
    };

    class BinOpNode : public INode
    {
        BinOpType opType_;
        INode* lChild_ = nullptr;
        INode* rChild_ = nullptr;

    public :
        BinOpNode(INode* par, BinOpType t, INode* l, INode* r) : INode{par, NodeType::BINOP},
                                                                 opType_{t},
                                                                 lChild_{l},
                                                                 rChild_{r} {};
                                                                 
        BinOpType get_binop_type() { return opType_; }
        
        const INode* get_left_child() { return lChild_; }

        const INode* get_right_child() {return rChild_; }
    }

    class StatementNode : public INode
    {
        BinOpNode* expression_ = nullptr;

    public :
        StatementNode(INode* par, BinOpNode* expr) : INode{par, NodeType::STATEMENT}, 
                                                     expression_(expr) {}
        const BinOpNode* get_expression() { return expression_; }  
    };

    class IfExpressionNode : public StatementNode
    {
        ScopeNode* scope_ = nullptr;
    };

    class WhileExpressionNode : public StatementNode
    {
        ScopeNode* scope_ = nullptr;
    };






}   //  namespace astnodes