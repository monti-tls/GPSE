#include "sketch/ast.hpp"

using namespace gpse;
using namespace sketch;

LiteralNode::LiteralNode(core::Literal const& value, lang::Node* parent)
    : lang::Node(LITERAL_NODE, parent)
    , _m_value(value)
{
}

LiteralNode::~LiteralNode()
{
}

core::Literal const& LiteralNode::value() const
{
    return _m_value;
}

VariableNode::VariableNode(core::Variable const& value, lang::Node* parent)
    : Node(VARIABLE_NODE, parent)
    , _m_value(value)
{
}

VariableNode::~VariableNode()
{
}

core::Variable const& VariableNode::value() const
{
    return _m_value;
}

FunctionCallNode::FunctionCallNode(core::Function const& value, lang::Node* parent)
    : Node(FUNCTION_CALL_NODE, parent)
    , _m_value(value)
{
}

FunctionCallNode::~FunctionCallNode()
{
}

core::Function const& FunctionCallNode::value() const
{
    return _m_value;
}

CallbackCallNode::CallbackCallNode(core::Callback const& value, lang::Node* parent)
    : Node(CALLBACK_CALL_NODE, parent)
    , _m_value(value)
{
}

CallbackCallNode::~CallbackCallNode()
{
}

core::Callback const& CallbackCallNode::value() const
{
    return _m_value;
}

CastNode::CastNode(core::Type const& to, lang::Node* expr, lang::Node* parent)
    : Node(CAST_NODE, parent)
    , _m_to(to)
{
    addChild(expr);
}

CastNode::~CastNode()
{
}

core::Type const& CastNode::to() const
{
    return _m_to;
}

ExpressionNode::ExpressionNode(ExpressionNode::Unary what, lang::Node* expr, lang::Node* parent)
    : lang::Node(EXPRESSION_NODE, parent)
    , _m_unary(what)
    , _m_binary((Binary)-1)
{
    addChild(expr);
}

ExpressionNode::ExpressionNode(ExpressionNode::Binary what, lang::Node* lhs, lang::Node* rhs, lang::Node* parent)
    : lang::Node(EXPRESSION_NODE, parent)
    , _m_unary((Unary)-1)
    , _m_binary(what)
{
    addChild(lhs);
    addChild(rhs);
}

ExpressionNode::~ExpressionNode()
{
}

bool ExpressionNode::isUnary() const
{
    return _m_unary != (Unary)-1;
}

bool ExpressionNode::isBinary() const
{
    return _m_binary != (Binary)-1;
}

ExpressionNode::Unary ExpressionNode::unaryOperation() const
{
    return _m_unary;
}

ExpressionNode::Binary ExpressionNode::binaryOperation() const
{
    return _m_binary;
}

VariableDeclNode::VariableDeclNode(core::Variable const& variable, lang::Node* value, lang::Node* parent)
    : lang::Node(VARIABLE_DECL_NODE, parent)
    , _m_variable(variable)
{
    if(value)
    {
        addChild(value);
    }
}

VariableDeclNode::~VariableDeclNode()
{
}

core::Variable const& VariableDeclNode::variable() const
{
    return _m_variable;
}

VariableAssignNode::VariableAssignNode(core::Variable const& variable, lang::Node* value, lang::Node* parent)
    : lang::Node(VARIABLE_ASSIGN_NODE, parent)
    , _m_variable(variable)
{
    addChild(value);
}

VariableAssignNode::~VariableAssignNode()
{
}

core::Variable const& VariableAssignNode::variable() const
{
    return _m_variable;
}

ReturnStatementNode::ReturnStatementNode(lang::Node* value, lang::Node* parent)
    : lang::Node(RETURN_STATEMENT_NODE, parent)
{
    if(value)
    {
        addChild(value);
    }
}

ReturnStatementNode::~ReturnStatementNode()
{
}

StatementBlockNode::StatementBlockNode(lang::Node* parent)
    : lang::Node(STATEMENT_BLOCK_NODE, parent)
{
}

StatementBlockNode::~StatementBlockNode()
{
}

FunctionDeclarationNode::FunctionDeclarationNode(core::Function const& function, lang::Node* parent)
    : lang::Node(FUNCTION_DECLARATION_NODE, parent)
    , _m_function(function)
{
}

FunctionDeclarationNode::~FunctionDeclarationNode()
{
}

core::Function const& FunctionDeclarationNode::function() const
{
    return _m_function;
}

ProgramNode::ProgramNode(lang::Node* parent)
    : lang::Node(PROGRAM_NODE, parent)
{
}

ProgramNode::~ProgramNode()
{
}
