//-------------------------------------------------------------------------------------------------
//
//  Nodes for abstract syntax tree
//
//-------------------------------------------------------------------------------------------------
#pragma once

#include <cassert>
#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include <string>

namespace astnodes
{
//-------------------------------------------------------------------------------------------------
//      TYPES        
    enum class BinOpType
    {
        ASSIGN,
        MINUS,
        PLUS,
        DIV,
        MUL,
        LESS,
        GREATER,
        EQUAL,
        LEQUAL,
        GEQUAL,
        NEQUAL
    };

//-------------------------------------------------------------------------------------------------
//      NODES       
    class INode
    {
    public :
        INode() = default;
        virtual ~INode() {}
    };

    class StatementINode : public INode
    {
    public:
        StatementINode() : INode{} {}
        virtual void execute() = 0;
    };

    class ExpressionINode : public INode
    {    
    public:
        ExpressionINode() : INode{} {}
        virtual int execute() = 0;    
    };

    class NumberNode final : public ExpressionINode
    {
        int number_;

    public :
        NumberNode() : ExpressionINode{} {}
        NumberNode(const int n) : ExpressionINode{}, number_(n) {}
        int execute() override { return number_; } 

        void set_value(const int n) { number_ = n; }
        int get_value() { return number_; } 
    };

    class VariableNode final : public ExpressionINode
    {
        std::string id_;
        int value_;

    public :
        VariableNode(const std::string i) : ExpressionINode{}, id_(i) {}
        int execute() override { return value_; }
        void set_value(const int v) { value_ = v; } 
    };

    class CurrentScopeNode : public INode
    {
        CurrentScopeNode* parent_ = nullptr;
        std::map<std::string, VariableNode*> context_;  //  symbol table for this scope
        std::vector<StatementINode*> curScope_;

    public:
        CurrentScopeNode(CurrentScopeNode* p) : INode{}, parent_(p) {}
        CurrentScopeNode() : INode{} {}
        CurrentScopeNode* get_parent_scope() { return parent_; }

        void add_statement(StatementINode* s) 
        { 
            assert(s);
            curScope_.push_back(s); 
        }
        void add_to_context(VariableNode* v) { /*....*/ }
        bool is_declared(const std::string id) { return (context_.find(id) == context_.end()); }

        auto begin() { return curScope_.begin(); }
        auto end() { return curScope_.end(); }
    };

    class ExpressionWrapper final : public StatementINode
    {
        ExpressionINode* expr_ = nullptr;

    public :
        ExpressionWrapper(ExpressionINode* e) : StatementINode{}, expr_(e) {}
        void execute() override { expr_->execute(); }
    };

    class BinOpWrapper : public ExpressionINode
    {
        ExpressionINode* expr_ = nullptr;

    public :
        BinOpWrapper(ExpressionINode* e) : ExpressionINode{}, expr_(e) {}
        int execute() { return expr_->execute(); }
    };
    
    class BinOpNode final : public ExpressionINode
    {
        ExpressionINode* leftExpr_ = nullptr;
        ExpressionINode* rightExpr_ = nullptr; 
        BinOpType opType_;
        
    public:
        BinOpNode(ExpressionINode* l, ExpressionINode* r, BinOpType t) : ExpressionINode{}, 
        leftExpr_(l),
        rightExpr_(r),
        opType_(t) {}
        int execute() override;
    };
    
    class IfExpressionNode final : public StatementINode
    {
        ExpressionINode* expr_ = nullptr;
        CurrentScopeNode* scope_ = nullptr;

    public:
        IfExpressionNode(ExpressionINode* e, CurrentScopeNode* s) : StatementINode{}, 
                                                                    expr_(e), scope_(s) {}
        void execute() override { expr_->execute(); }
    };

    class WhileExpressionNode final : public StatementINode
    {
        ExpressionINode* expr_ = nullptr;
        CurrentScopeNode* scope_ = nullptr;

    public:
        WhileExpressionNode(ExpressionINode* e, CurrentScopeNode* s) : StatementINode{}, 
                                                                       expr_(e), scope_(s) {}
        void execute() override { expr_->execute(); }
    };

    class AssignExpressionNode final : public ExpressionINode
    {
        VariableNode* var_ = nullptr;
        ExpressionINode* expr_ = nullptr;

        public :
        AssignExpressionNode(VariableNode* v, ExpressionINode* e) : ExpressionINode{}, 
        var_(v), expr_(e) {}
        int execute()
        {
            int value = expr_->execute();
            var_->set_value(value);
            return value;
        }
    };

    class PrintNode final : public ExpressionINode
    {
        ExpressionINode* expr_ = nullptr;

    public :
        PrintNode(ExpressionINode* e) : ExpressionINode{}, expr_(e) {}
        int execute() 
        { 
            int prValue = expr_->execute();
            std::cout << prValue << std::endl;
            return prValue; 
        }
    };
    
    class InputNode final : public ExpressionINode
    {
        NumberNode* value_ = nullptr;

        public :
        InputNode(NumberNode* n) : ExpressionINode{}, value_(n) {}
        int execute() override
        {
            int number;
            std::cin >> number;
            value_->set_value(number);
            return value_->get_value();
        }
    };
//-------------------------------------------------------------------------------------------------
//      MAKE NODE FUNCS
template <typename NodeType, class... Args>
NodeType* make_node(Args&&... args)
{
    std::unique_ptr<NodeType> pNode = std::make_unique<NodeType>(args ...);
    return pNode.get();
}
}   //  namespace astnodes