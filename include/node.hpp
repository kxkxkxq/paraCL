#pragma once

#include <vector>
#include <string>

namespace astnodes
{
//-------------------------------------------------------------------------------------------------
//      NODE TYPES    
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

//-------------------------------------------------------------------------------------------------
//      NODES    
    class INode
    {
        NodeType nType_;
    
    public:
        INode(NodeType t) = default;
        NodeType get_node_type() { return nType_; }
        virtual ~INode() {}
    };

    class IdNode final : public INode
    {
        std::string id_;

    public :
        IdNode(std::string i) : INode{NodeType::ID}, id_(i) {}
        std::string get_id() { return id_; }
    };

    class NumberNode final : public INode
    {
        int number_;

    public : 
        NumberNode(int n) : INode{NodeType::NUMBER}, number_(n) {}
        int get_number() { return number_; }  
    };

    class ScopeNode final: public INode
    {
        std::vector<INode*> scopeVec_;

    public : 
        ScopeNode() : INode{ NodeType::SCOPE } {}
        void push_node_to_scope (INode* n) { scopeVec_.push_back(n); } 
    };

    class BinOpNode final : public INode
    {
        INode* lChild_ = nullptr;
        INode* rChild_ = nullptr;
        BinOpType opType_;

    public :
        BinOpNode(INode* l, INode* r, BinOpType t) : INode{NodeType::BINOP}, 
                                                     rChild_{l}, 
                                                     lChild_{r}, 
                                                     opType_{t} {}                                                       
        BinOpType get_binop_type() { return opType_; }
        INode* get_left_child() { return lChild_; }
        INode* get_right_child() {return rChild_; }
    };

    class StatementNode : public INode
    {
        BinOpNode* expression_ = nullptr;

    public :
        StatementNode(BinOpNode* expr) : INode{NodeType::STATEMENT}, expression_(expr) {}
        BinOpNode* get_bin_op_expression() { return expression_; }  

    private :
        virtual StatementType get_statement_type() const = 0;
    };

    class ExpressionNode final : public StatementNode
    {
        StatementType stType_;

    public :
        ExpressionNode(BinOpNode* expr) : StatementNode{expr}, stType_(StatementType::EXPRESSION) {}
        StatementType get_statement_type() const override { return stType_; }
    };

    class IfExpressionNode final : public StatementNode
    {
        StatementType stType_;
        ScopeNode* scope_ = nullptr;

    public :
        IfExpressionNode(BinOpNode* expr, ScopeNode* sc) : StatementNode{expr},
                                                           stType_(StatementType::IF),
                                                           scope_(sc) {}
        StatementType get_statement_type() const override { return stType_; }                                                
    };

    class WhileExpressionNode final : public StatementNode
    {
        StatementType stType_;
        ScopeNode* scope_ = nullptr;
    public :
        WhileExpressionNode(BinOpNode* expr, ScopeNode* sc) : StatementNode{expr},
                                                              stType_(StatementType::WHILE),
                                                              scope_(sc) {} 
        StatementType get_statement_type() const override { return stType_; }
    };

    class Statements final
    {
        std::vector<StatementNode*> statVec_;

    public :
        Statements() {}
        void add_statement(StatementNode* sn) { statVec_.push_back(sn); }
    }

//-------------------------------------------------------------------------------------------------
//      MAKE NODE FUNCS
IdNode*         make_id_node(std::string s) { return new IdNode{s}; }
NumberNode*     make_number_node(int n) { return new NumberNode{n}; } 
BinOpNode*      make_bin_op_node(INode* l, INode* r, BinOpType t) { return new BinOpNode{l, r, t}; } 
ExpressionNode* make_expression_node(BinOpNode* e) { return new ExpressionNode{e}; }   
}   //  namespace astnodes