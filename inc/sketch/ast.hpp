#ifndef __SKETCH_AST_H__
#define __SKETCH_AST_H__

#include "lang/node.hpp"
#include "core/type.hpp"
#include "core/variable.hpp"
#include "core/function.hpp"
#include "core/literal.hpp"

namespace gpse
{
    namespace sketch
    {
        enum
        {
            LITERAL_NODE,
            EXPRESSION_NODE,
            VARIABLE_NODE,
            FUNCTION_CALL_NODE,
            CALLBACK_CALL_NODE,
            CAST_NODE,
            VARIABLE_DECL_NODE,
            VARIABLE_ASSIGN_NODE,
            RETURN_STATEMENT_NODE,
            STATEMENT_BLOCK_NODE,
            FUNCTION_DECLARATION_NODE,
            IF_BLOCK_NODE,
            ELIF_BLOCK_NODE,
            ELSE_BLOCK_NODE,
            CONDITIONAL_BLOCK_NODE,
            WHILE_BLOCK_NODE,
            PROGRAM_NODE
        };

        class LiteralNode : public lang::Node
        {
        public:
            LiteralNode(core::Literal const& value, lang::Node* parent = nullptr);
            ~LiteralNode();

            core::Literal const& value() const;

        private:
            core::Literal _m_value;
        };

        class VariableNode : public lang::Node
        {
        public:
            VariableNode(core::Variable const& value, lang::Node* parent = nullptr);
            ~VariableNode();

            core::Variable const& value() const;

        private:
            core::Variable _m_value;
        };

        class FunctionCallNode : public lang::Node
        {
        public:
            FunctionCallNode(core::Function const& value, lang::Node* parent = nullptr);
            ~FunctionCallNode();

            core::Function const& value() const;

        private:
            core::Function _m_value;
        };

        class CallbackCallNode : public lang::Node
        {
        public:
            CallbackCallNode(core::Callback const& value, lang::Node* parent = nullptr);
            ~CallbackCallNode();

            core::Callback const& value() const;

        private:
            core::Callback _m_value;
        };

        class CastNode : public lang::Node
        {
        public:
            CastNode(core::Type const& to, lang::Node* expr, lang::Node* parent = nullptr);
            ~CastNode();

            core::Type const& to() const;

        private:
            core::Type _m_to;
        };

        class ExpressionNode : public lang::Node
        {
        public:
            enum class Unary
            {
                Not,
                Neg
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
            ExpressionNode(Unary what, lang::Node* expr, lang::Node* parent = nullptr);
            ExpressionNode(Binary what, lang::Node* lhs, lang::Node* rhs, lang::Node* parent = nullptr);
            ~ExpressionNode();

            bool isUnary() const;
            bool isBinary() const;

            Unary unaryOperation() const;
            Binary binaryOperation() const;

        private:
            Unary _m_unary;
            Binary _m_binary;
        };

        class VariableDeclNode : public lang::Node
        {
        public:
            VariableDeclNode(core::Variable const& variable, lang::Node* initialization = nullptr, lang::Node* parent = nullptr);
            ~VariableDeclNode();

            core::Variable const& variable() const;

        private:
            core::Variable _m_variable;
        };

        class VariableAssignNode : public lang::Node
        {
        public:
            VariableAssignNode(core::Variable const& variable, lang::Node* value, lang::Node* parent = nullptr);
            ~VariableAssignNode();

            core::Variable const& variable() const;

        private:
            core::Variable _m_variable;
        };

        class ReturnStatementNode : public lang::Node
        {
        public:
            ReturnStatementNode(lang::Node* value = nullptr, lang::Node* parent = nullptr);
            ~ReturnStatementNode();
        };

        class StatementBlockNode : public lang::Node
        {
        public:
            StatementBlockNode(lang::Node* parent = nullptr);
            ~StatementBlockNode();
        };

        class FunctionDeclarationNode : public lang::Node
        {
        public:
            FunctionDeclarationNode(core::Function const& function, lang::Node* parent = nullptr);
            ~FunctionDeclarationNode();

            core::Function const& function() const;

        private:
            core::Function _m_function;
        };

        class IfBlockNode : public lang::Node
        {
        public:
            IfBlockNode(lang::Node* condition, lang::Node* block, lang::Node* parent = nullptr);
            ~IfBlockNode();

            lang::Node*& condition();
            lang::Node*& block();
        };

        class ElifBlockNode : public lang::Node
        {
        public:
            ElifBlockNode(lang::Node* condition, lang::Node* block, lang::Node* parent = nullptr);
            ~ElifBlockNode();

            lang::Node*& condition();
            lang::Node*& block();
        };

        class ElseBlockNode : public lang::Node
        {
        public:
            ElseBlockNode(lang::Node* block, lang::Node* parent = nullptr);
            ~ElseBlockNode();

            lang::Node*& block();
        };

        class ConditionalBlockNode : public lang::Node
        {
        public:
            ConditionalBlockNode(lang::Node* parent = nullptr);
            ~ConditionalBlockNode();
        };

        class WhileBlockNode : public lang::Node
        {
        public:
            WhileBlockNode(lang::Node* condition, lang::Node* block, lang::Node* parent = nullptr);
            ~WhileBlockNode();

            lang::Node*& condition();
            lang::Node*& block();
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
