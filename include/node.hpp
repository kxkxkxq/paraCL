#pragma once

#include <vector>
#include <string>

namespace astnodes
{
//-------------------------------------------------------------------------------------------------
//      TYPES    
    enum class NodeType
    {
        BINOP,
        SCOPE,
        STATEMENT
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

//-------------------------------------------------------------------------------------------------
//      NODES    
    class INode
    {
        NodeType nType_;
    
    public:
        INode(NodeType t) : nType_(t) {};
        NodeType get_node_type() { return nType_; }
        virtual ~INode() {}
    };

    class BinOpNode final : public INode
    {
        INode* lChild_ = nullptr;
        INode* rChild_ = nullptr;
        BinOpType opType_;
        
    public :
        BinOpNode(INode* l, INode* r, BinOpType t) : INode{NodeType::BINOP}, 
                                                           lChild_{l}, 
                                                           rChild_{r}, 
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

    class IfExpressionNode final : public StatementINode
    {
        ExpressionINode* expr_ = nullptr;
        ScopeNode* scope_ = nullptr;

    public :
        IfExpressionNode(ExpressionINode* e, ScopeNode* s) : StatementINode{StatementType::IF},
                                                             expr_(e), scope_(s) {} 
        //  get_expression()
        //  get_scope()                                           
    };

    class WhileExpressionNode final : public StatementINode
    {
        ExpressionINode* expr_ = nullptr;
        ScopeNode* scope_ = nullptr; 
       
    public :
        WhileExpressionNode(ExpressionINode* e, ScopeNode* s) : StatementINode{StatementType::WHILE},
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

        template< class InputIt >
        void assign( InputIt first, InputIt last ) { statVec_.assign(first, last); } 
        void add_statement(StatementINode* sn) { statVec_.push_back(sn); }
        
        auto begin() { return statVec_.begin(); }
        auto end() { return statVec_.end(); }
    };

//-------------------------------------------------------------------------------------------------
//      MAKE NODE FUNCS
IdNode*         make_id_node(std::string s) { return new IdNode{s}; }
NumberNode*     make_number_node(int n) { return new NumberNode{n}; } 
BinOpNode*      make_bin_op_node(INode* l, INode* r, BinOpType t) { return new BinOpNode{l, r, t}; } 
ExpressionINode* make_expression_node(BinOpNode* e) { return new ExpressionINode{e}; }   
}   //  namespace astnodes