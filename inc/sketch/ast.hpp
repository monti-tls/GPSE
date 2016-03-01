#ifndef __SKETCH_AST_H__
#define __SKETCH_AST_H__

#include "lang/node.hpp"
#include "core/object.hpp"
#include "core/symbol.hpp"

namespace gpse
{
    namespace sketch
    {
        enum
        {
            EXPR_LITERAL_NODE,
            EXPR_SYMBOL_NODE,
            EXPR_NODE,

            STMT_ASSIGN_NODE,
            STMT_RETURN_NODE,
            STMT_BLOCK_NODE,

            DECL_VARIABLE_NODE,
            /*DECL_FUNCTION_NODE,

            BLOCK_IF_NODE,
            BLOCK_ELIF_NODE,
            BLOCK_ELSE_NODE,
            BLOCK_CONDITIONAL_NODE,

            BLOCK_WHILE_NODE,*/

            PROGRAM_NODE
        };

        class ExprLiteralNode : public lang::Node
        {
        public:
            ExprLiteralNode(core::Object const& value, lang::Node* parent = nullptr);

            core::Object const& value() const;

        private:
            core::Object m_value;
        };

        class ExprSymbolNode : public lang::Node
        {
        public:
            ExprSymbolNode(core::Symbol* symbol, lang::Node* parent = nullptr);

            core::Symbol* symbol() const;

        private:
            core::Symbol*  m_symbol;
        };

        class ExprNode : public lang::Node
        {
        public:
            enum class Unary
            {
                Not,
                Neg,
                Call
            };

            enum class Binary
            {
                Mul,
                Div,
                Add,
                Sub,
                Lt,
                Lte,
                Gt,
                Gte,
                Eq,
                Neq,
                And,
                Or
            };

        public:
            ExprNode(Unary what, lang::Node* expr, lang::Node* parent = nullptr);
            ExprNode(Binary what, lang::Node* lhs, lang::Node* rhs, lang::Node* parent = nullptr);

            bool isUnary() const;
            bool isBinary() const;

            Unary unaryOperation() const;
            Binary binaryOperation() const;

            ExprNode* lhs() const;
            ExprNode* rhs() const;

        private:
            Unary m_unary;
            Binary m_binary;
        };

        class StmtAssignNode : public lang::Node
        {
        public:
            StmtAssignNode(lang::Node* lvalue, lang::Node* rvalue, lang::Node* parent = nullptr);

            ExprNode* lvalue() const;
            ExprNode* rvalue() const;
        };

        class StmtReturnNode : public lang::Node
        {
        public:
            StmtReturnNode(lang::Node* value = nullptr, lang::Node* parent = nullptr);

            ExprNode* value() const;
        };

        class StmtBlockNode : public lang::Node
        {
        public:
            StmtBlockNode(lang::Node* parent = nullptr);
        };

        class DeclVariableNode : public lang::Node
        {
        public:
            DeclVariableNode(core::Symbol* symbol, lang::Node* init = nullptr, lang::Node* parent = nullptr);

            core::Symbol* symbol() const;
            ExprNode* init() const;

        private:
            core::Symbol* m_symbol;
        };

        class ProgramNode : public lang::Node
        {
        public:
            ProgramNode(lang::Node* parent = nullptr);
            ~ProgramNode();
        };
    }
}

#endif // __SKETCH_AST_H__
