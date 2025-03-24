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

namespace ast
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
        VariableNode(const std::string i) : ExpressionINode{}, id_(i) { }
        int execute() override { return value_; }
        void set_value(const int v) { value_ = v; } 
    };

    class CurrentScopeNode : public StatementINode
    {
        CurrentScopeNode* parent_ = nullptr;
        std::map<std::string, VariableNode*> context_;  //  symbol table for current scope
        std::vector<StatementINode*> curScope_;

    public:
        CurrentScopeNode(CurrentScopeNode* p) : StatementINode{}, parent_(p) {}
        CurrentScopeNode() : StatementINode{} {}
        CurrentScopeNode* get_parent_scope() { return parent_; }

        void add_to_context(VariableNode* v) { /*....*/ }
        bool is_declared(const std::string id) { return (context_.find(id) == context_.end()); }
        void add_statement(StatementINode* s) 
        { 
            StatementINode* tmp = s;
            assert(s);
            curScope_.emplace_back(s);
            assert(tmp == curScope_.back()); 
        }
        
        void execute() override
        {
            for(auto&& stmnt : curScope_)
            {
                assert(stmnt);
                stmnt->execute();
            }
        }
    };

    class ExpressionWrapper final : public StatementINode
    {
        ExpressionINode* expr_ = nullptr;

    public :
        ExpressionWrapper(ExpressionINode* e) : StatementINode{}, expr_(e) { }
        void execute() override {  assert(expr_) ; expr_->execute(); }
    };

    class StatementWrapper final : public StatementINode
    {
        StatementINode* stmnt_ = nullptr;

    public :
        StatementWrapper(StatementINode* s) : StatementINode{}, stmnt_(s) { }
        void execute() override {  assert(stmnt_) ; stmnt_->execute(); }
    };
    

    class ArithmExprWrapper : public ExpressionINode
    {
        ExpressionINode* expr_ = nullptr;

    public :
        ArithmExprWrapper(ExpressionINode* e) : ExpressionINode{}, expr_(e) { }
        int execute() override { return expr_->execute(); }
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
        int execute() override 
        { 
            assert(leftExpr_);
            assert(rightExpr_);
            leftExpr_->execute(); 
            rightExpr_->execute();
#if 0            
            switch(BinOpType):
            {
                case :
            }
#endif            
        }
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
            assert(var_);
            assert(expr_);
            int value = expr_->execute();
            var_->set_value(value);
            return var_->execute();
        }
    };

    class PrintNode final : public ExpressionINode
    {
        ExpressionINode* expr_ = nullptr;

    public :
        PrintNode(ExpressionINode* e) : ExpressionINode{}, expr_(e) {}
        int execute() 
        { 
            assert(expr_);
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
}   //  namespace ast