#ifndef __SKETCH_AST_H__
#define __SKETCH_AST_H__

#include "lang/node.hpp"
#include "core/type.hpp"
#include "core/literal.hpp"
#include "core/variablename.hpp"

namespace gpse
{
  namespace sketch
  {
    enum
    {
      LITERAL_NODE,
      EXPRESSION_NODE,
      VARIABLE_NODE,
      VARIABLE_DECL_NODE,
      VARIABLE_ASSIGN_NODE,
      STATEMENT_BLOCK_NODE
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
      VariableNode(core::VariableName const& value, lang::Node* parent = nullptr);
      ~VariableNode();
      
      core::VariableName const& value() const;
      
    private:
      core::VariableName _m_value;
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
      ExpressionNode(Unary what, ExpressionNode* expr, lang::Node* parent = nullptr);
      ExpressionNode(Binary what, ExpressionNode* lhs, ExpressionNode* rhs, lang::Node* parent = nullptr);
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
      VariableDeclNode(core::Type const& type, core::VariableName const& name, ExpressionNode* initialization = nullptr, lang::Node* parent = nullptr);
      ~VariableDeclNode();
      
      core::Type const& type() const;
      core::VariableName const& name() const;
      
    private:
      core::Type _m_type;
      core::VariableName _m_name;
    };
    
    class VariableAssignNode : public lang::Node
    {
    public:
      VariableAssignNode(core::VariableName const& name, ExpressionNode* value, lang::Node* parent = nullptr);
      ~VariableAssignNode();
      
      core::VariableName const& name() const;
      
    private:
      core::VariableName _m_name;
    };
    
    class StatementBlockNode : public lang::Node
    {
    public:
      StatementBlockNode(lang::Node* parent = nullptr);
      ~StatementBlockNode();
    };
  }
}

#endif // __SKETCH_AST_H__
