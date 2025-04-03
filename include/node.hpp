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
        MINUS,
        PLUS,
        DIV,
        MUL,
        LESS,
        GREATER,
        MOD,
        EQUAL,
        LEQUAL,
        GEQUAL,
        NEQUAL,
        AND,
        OR
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

    public:
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

    public:
        VariableNode(const std::string i) : ExpressionINode{}, id_(i) { }
        
        int execute() override { return value_; }
        
        std::string get_id() const { return id_; } 
        void set_value(const int v) { value_ = v; } 
    };

    class CurrentScopeNode : public StatementINode
    {
        std::map<std::string, VariableNode*> context_;  //  symbol table for current scope
        std::vector<StatementINode*> curScope_;

    public:
        CurrentScopeNode() : StatementINode{} {}

        void add_to_context(VariableNode* var) 
        { 
            assert(var);
            context_.insert({var->get_id(), var});
            assert(context_.find(var->get_id()) != context_.end());
            assert(context_.find(var->get_id())->second == var);
        }        

        bool is_declared(const std::string id) const 
        {  
            if(context_.find(id) == context_.end()) 
                return false;
            return true;
        }

        VariableNode* get_variable(std::string id)
        {
            auto iter = context_.find(id);
            assert(iter != context_.end());
            return iter->second;
        }
        
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

    public:
        ExpressionWrapper(ExpressionINode* e) : StatementINode{}, expr_(e) { }
        void execute() override {  assert(expr_) ; expr_->execute(); }
    };
    
    class StatementWrapper final : public StatementINode
    {
        StatementINode* stmnt_ = nullptr;

    public:
        StatementWrapper(StatementINode* s) : StatementINode{}, stmnt_(s) { }
        void execute() override {  assert(stmnt_) ; stmnt_->execute(); }
    };

    class EmptyStatement final : public StatementINode
    {
    public:
        EmptyStatement() : StatementINode{} {}
        void execute() override { return; }
    };

    class ArithmExprWrapper : public ExpressionINode
    {
        ExpressionINode* expr_;
        bool exprSign_;

    public:
        ArithmExprWrapper(ExpressionINode* e = nullptr, 
                          BinOpType s = ast::BinOpType::PLUS) : ExpressionINode{}, 
                                                                expr_(e),
                                                                exprSign_(static_cast<bool>(s)) {}
        int execute() override 
        { 
            assert(expr_);
            return (exprSign_) ? expr_->execute() : -(expr_->execute()); 
        }
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
            int lExprRes = leftExpr_->execute(); 
            int rExprRes = rightExpr_->execute();

            switch(opType_)
            {
                case BinOpType::MINUS:    return lExprRes  - rExprRes;
                case BinOpType::PLUS:     return lExprRes  + rExprRes;
                case BinOpType::DIV:      {
                                            if(rExprRes == 0) 
                                                throw std::overflow_error("runtime error: division by zero"); 
                                            return lExprRes  / rExprRes;
                                          }
                case BinOpType::MUL:      return lExprRes  * rExprRes;
                case BinOpType::LESS:     return lExprRes  < rExprRes;
                case BinOpType::GREATER:  return lExprRes  > rExprRes;
                case BinOpType::MOD:      {
                                            if(rExprRes == 0) 
                                                throw std::overflow_error("runtime error: division by zero"); 
                                            return lExprRes  % rExprRes;
                                          }
                case BinOpType::EQUAL:    return lExprRes == rExprRes;
                case BinOpType::LEQUAL:   return lExprRes <= rExprRes;
                case BinOpType::GEQUAL:   return lExprRes >= rExprRes;
                case BinOpType::NEQUAL:   return lExprRes != rExprRes;
                case BinOpType::AND:      return lExprRes && rExprRes;
                case BinOpType::OR:       return lExprRes || rExprRes;
            } 
            throw std::runtime_error("impossible case during executing a binary operation");        
        }
    };

    class IfExpressionNode final : public StatementINode
    {
        ExpressionINode* expr_ = nullptr;
        StatementWrapper* ifScope_ = nullptr;

    public:
        IfExpressionNode(ExpressionINode* e, StatementWrapper* s) : StatementINode{}, 
                                                                    expr_(e), ifScope_(s) {}
        void execute() override 
        {
            assert(expr_);
            assert(ifScope_);
            if(expr_->execute())
                ifScope_->execute();
        }
    };

    class WhileExpressionNode final : public StatementINode
    {
        ExpressionINode* expr_ = nullptr;
        StatementWrapper* whileScope_ = nullptr;

    public:
        WhileExpressionNode(ExpressionINode* e, StatementWrapper* s) : StatementINode{}, 
                                                                       expr_(e), whileScope_(s) {}
        void execute() override 
        { 
            assert(expr_);
            assert(whileScope_);
            while(expr_->execute())
                whileScope_->execute(); 
        }
    };

    class AssignExpressionNode final : public ExpressionINode
    {
        VariableNode* var_ = nullptr;
        ExpressionINode* expr_ = nullptr;

    public:
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

    public:
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

    public:
        InputNode(NumberNode* n) : ExpressionINode{}, value_(n) {}
        
        int execute() override
        {
            assert(value_);
            int number;
            std::cin >> number;
            if(std::cin.fail())
            {
                std::string buffer;
                std::cin.clear();
                std::cin >> buffer;
                throw std::runtime_error("runtime error: incorrect input, unexpected '"
                                         + buffer + "', expected integer number");
            }
            value_->set_value(number);
            return value_->get_value();
        }
    };
}   //  namespace ast