#pragma once

#include <iostream>
#include <vector>
#include <string>

namespace astnodes
{
//-------------------------------------------------------------------------------------------------
//      TYPES    
#if 0    
    enum class NodeType
    {
        BINOP,
        SCOPE,
        STATEMENT
    };
 #endif
    
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

#if 0
    enum class StatementType
    {
        EXPRESSION,
        IF,
        WHILE,
        PRINT
    };

    enum class ExpressionType
    {
        ASSIGNMENT,
        ARITHMETIC,
        TERMINAL
    };

    enum class TerminalType
    {
        VARIABLE,
        NUMBER
    };

    enum class VariableType
    {
        ID
    };
#endif

//-------------------------------------------------------------------------------------------------
//      NODES    
#if 1    
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

    class ExpressionWrapper final : public StatementINode
    {
        ExpressionINode* expr_;
    public :
        ExpressionWrapper() : StatementINode{} {}
        void execute() override { expr_->execute(); }
    };

    class CurrentScopeNode : public INode
    {
        std::vector<StatementINode*> curScope_;

    public:
        void push_statement(StatementINode* s) { curScope_.push_back(s); }
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

    class VariableNode final : public ExpressionINode
    {
        std::string id_;
        int value_;

    public :
        VariableNode(const std::string i) : ExpressionINode{}, id_(i) {}
        int execute() override { return value_; }
        void set_value(const int v) { value_ = v; } 
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
#endif

#if 0
    class INode
    {
        NodeType t;

    public :
        INode(NodeType t) : t(t) {};
        virtual ~INode() {}
    };

    class BinOpNode final : public INode
    {
        INode* lChild_ = nullptr;
        INode* rChild_ = nullptr;
        BinOpType opType_;
        
    public :
        BinOpNode(INode* l, INode* r, BinOpType t) : INode{NodeType::BINOP}, 
                                                     lChild_{l}, rChild_{r}, 
                                                     opType_{t} {}                                                       
        BinOpType get_binop_type() { return opType_; }
        INode* get_left_child() { return lChild_; }
        INode* get_right_child() {return rChild_; }
    };

    class ScopeNode final: public INode
    {
        std::vector<INode*> scopeVec_;

    public : 
        ScopeNode() : INode{ NodeType::SCOPE } {}
        template<class InputIt>
        void assign( InputIt first, InputIt last ) { scopeVec_.assign(first, last); } 
        void push_node_to_scope (INode* n) { scopeVec_.push_back(n); } 
    };

    class StatementINode : public INode
    {
        StatementType stType_;

    public :
        StatementINode(StatementType t) : INode{NodeType::STATEMENT} {}
        StatementType get_statement_type() const { return stType_; }
    };

    class ExpressionINode : public StatementINode
    {
        ExpressionType exprType_;

    public :
        ExpressionINode(ExpressionType t) : StatementINode{StatementType::EXPRESSION} {}
        ExpressionType get_expression_type() const { return exprType_; };
    };

    class IfExprNode final : public StatementINode
    {
        ExpressionINode* expr_ = nullptr;
        ScopeNode* scope_ = nullptr;

    public :
        IfExprNode(ExpressionINode* e, ScopeNode* s) : StatementINode{StatementType::IF},
                                                             expr_(e), scope_(s) {} 
        //  get_expression()
        //  get_scope()                                           
    };

    class WhileExprNode final : public StatementINode
    {
        ExpressionINode* expr_ = nullptr;
        ScopeNode* scope_ = nullptr; 
       
    public :
        WhileExprNode(ExpressionINode* e, ScopeNode* s) : StatementINode{StatementType::WHILE},
                                                                expr_(e), scope_(s) {}
        //  get_expression()
        //  get_scope()   
    };

    class AssignmentExprNode final : public ExpressionINode
    {
        BinOpNode* expr_ = nullptr;
    
    public :
        AssignmentExprNode(BinOpNode* e) : ExpressionINode{ExpressionType::ASSIGNMENT}, expr_(e) {}
        //  get_expression()
    };

    class PrintNode final : public StatementINode 
    {
        ExpressionINode* expr_ = nullptr;

    public:
        PrintNode(ExpressionINode* e) : StatementINode{StatementType::PRINT}, expr_(e) {}
        //  get expression()
    };

    class ArithmeticExprNode final : public ExpressionINode
    {
        BinOpNode* expr_ = nullptr;
    
    public :
        ArithmeticExprNode(BinOpNode* e) : ExpressionINode{ExpressionType::ARITHMETIC}, expr_(e) {}
        //  get_expression()
    };

    class TerminalINode : public ExpressionINode
    {
        TerminalType termType_;
    
    public :
        TerminalINode(TerminalType t) : ExpressionINode{ExpressionType::TERMINAL} {}
        TerminalType get_terminal_type() const { return termType_; };
    };

    class NumberNode final : public TerminalINode
    {
        int number_;

    public :
        NumberNode(int n) : TerminalINode{TerminalType::NUMBER}, number_(n) {}
        int get_number() { return number_; } 
    };

    class VariableINode : public TerminalINode
    {
        VariableType varType_;

    public :
        VariableINode(VariableType t) : TerminalINode{TerminalType::VARIABLE} {}
        VariableType get_varible_type() { return varType_; }
    };

    class IdNode final : public VariableINode
    {
        std::string id_;

    public :
        IdNode(std::string i) : VariableINode{VariableType::ID}, id_(i) {}
        std::string get_id() { return id_; } 
    };

    class Statements final
    {
        std::vector<StatementINode*> statVec_;

    public :
        Statements() {}

        template<class InputIt>
        void assign( InputIt first, InputIt last ) { statVec_.assign(first, last); } 
        void add_statement(StatementINode* sn) { statVec_.push_back(sn); }
        
        auto begin() { return statVec_.begin(); }
        auto end() { return statVec_.end(); }
    };
#endif

//-------------------------------------------------------------------------------------------------
//      MAKE NODE FUNCS
#if 0
inline ScopeNode* 
  make_scope_node() { return new ScopeNode{}; }
inline IfExprNode* 
  make_if_node(ExpressionINode* e, ScopeNode* s) { return new IfExprNode{e, s}; }
inline WhileExprNode* 
  make_while_node(ExpressionINode* e, ScopeNode* s) { return new WhileExprNode{e, s}; }
inline BinOpNode* 
  make_bin_op_node(INode* l, INode* r, BinOpType t) { return new BinOpNode{l, r, t}; }
inline AssignmentExprNode* 
  make_assignment_node(BinOpNode* e) { return new AssignmentExprNode{e}; }
inline ArithmeticExprNode* 
  make_arithmetic_node(BinOpNode* e) { return new ArithmeticExprNode{e}; }
inline NumberNode* 
  make_number_node(int n) { return new NumberNode{n}; }
inline IdNode* 
  make_id_node(std::string i) { return new IdNode{i}; }
inline PrintNode*
  make_print_node(ExpressionINode* e) {return new PrintNode{e}; }
#endif
}   //  namespace astnodes