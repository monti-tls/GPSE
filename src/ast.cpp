#include "sketch/ast.hpp"

using namespace gpse;
using namespace sketch;

LiteralNode::LiteralNode(core::Literal const& value, lang::Node* parent) :
  lang::Node(LITERAL_NODE, parent),
  _m_value(value)
{
  
}

LiteralNode::~LiteralNode()
{
  
}

core::Literal const & LiteralNode::value() const
{
  return _m_value;
}

VariableNode::VariableNode(core::VariableName const& value, lang::Node* parent) :
  Node(VARIABLE_NODE, parent),
  _m_value(value)
{

}

VariableNode::~VariableNode()
{

}

core::VariableName const& VariableNode::value() const
{
  return _m_value;
}



ExpressionNode::ExpressionNode(ExpressionNode::Unary what, ExpressionNode* expr, lang::Node* parent) :
  lang::Node(EXPRESSION_NODE, parent),
  _m_unary(what),
  _m_binary((Binary) -1)
{
  addChild(expr);
}

ExpressionNode::ExpressionNode(ExpressionNode::Binary what, ExpressionNode* lhs, ExpressionNode* rhs, lang::Node* parent) :
  lang::Node(EXPRESSION_NODE, parent),
  _m_unary((Unary) -1),
  _m_binary(what)
{
  addChild(lhs);
  addChild(rhs);
}

ExpressionNode::~ExpressionNode()
{
  
}

bool ExpressionNode::isUnary() const
{
  return _m_unary != (Unary) -1;
}

bool ExpressionNode::isBinary() const
{
  return _m_binary != (Binary) -1;
}

ExpressionNode::Unary ExpressionNode::unaryOperation() const
{
  return _m_unary;
}

ExpressionNode::Binary ExpressionNode::binaryOperation() const
{
  return _m_binary;
}

VariableDeclNode::VariableDeclNode(core::Type const& type, core::VariableName const& name, ExpressionNode* value, lang::Node* parent) :
  lang::Node(VARIABLE_DECL_NODE, parent),
  _m_type(type),
  _m_name(name)
{
  if (value)
  {
    addChild(value);
  }
}

VariableDeclNode::~VariableDeclNode()
{
  
}

core::Type const& VariableDeclNode::type() const
{
  return _m_type;
}

const core::VariableName& VariableDeclNode::name() const
{
  return _m_name;
}

VariableAssignNode::VariableAssignNode(core::VariableName const& name, ExpressionNode* value, lang::Node* parent) :
  lang::Node(VARIABLE_ASSIGN_NODE, parent),
  _m_name(name)
{
  addChild(value);
}

VariableAssignNode::~VariableAssignNode()
{
  
}

core::VariableName const& VariableAssignNode::name() const
{
  return _m_name;
}

StatementBlockNode::StatementBlockNode(lang::Node* parent) :
  lang::Node(STATEMENT_BLOCK_NODE, parent)
{

}

StatementBlockNode::~StatementBlockNode()
{

}
