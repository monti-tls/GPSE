#include "sketch/pass_def.hpp"
#include "sketch/ast.hpp"
#include "lang/parser.hpp"
#include "core/symbol.hpp"
#include "core/type.hpp"
#include "core/function.hpp"

#include <iostream>

namespace gpse
{
  namespace sketch
  { 
    lang::TreePass getOptimizePass()
    {
      lang::TreePass pass;
      
      pass.addOperator<LITERAL_NODE, LiteralNode>(
        [](lang::TreePass* pass, lang::Node*& node, LiteralNode* literal)
        {
          // a literal node is already reduced
        }
      );
      
      pass.addOperator<CAST_NODE, CastNode>(
        [](lang::TreePass* pass, lang::Node*& node, CastNode* cast)
        {
          pass->pass(cast->children()[0]);
          
          if (cast->children()[0]->which() == LITERAL_NODE)
          {
            core::Literal const& value = static_cast<LiteralNode*>(cast->children()[0])->value();
            core::Literal newValue = value;
            
            if (cast->to() == core::Type::Integer)
            {
              newValue = value.integer();
            }
            else if (cast->to() == core::Type::Floating)
            {
              newValue = value.floating();
            }
            else if (cast->to() == core::Type::String)
            {
              newValue = value.string();
            }
            else if (cast->to() == core::Type::Boolean)
            {
              newValue = value.boolean();
            }
            
            LiteralNode* reduced = new LiteralNode(newValue);
            reduced->setParent(cast->parent());
            delete cast;
            node = reduced;
          }
        }
      );
      
      pass.addOperator<EXPRESSION_NODE, ExpressionNode>(
        [](lang::TreePass* pass, lang::Node*& node, ExpressionNode* expression)
        {
          // Simplify unary operations
          if (expression->isUnary())
          {
            lang::Node*& termNode = expression->children()[0];
            pass->pass(termNode);
            if (termNode->which() == LITERAL_NODE)
            {
              LiteralNode* term = static_cast<LiteralNode*>(termNode);
              LiteralNode* reduced;
              
              switch (expression->unaryOperation())
              {
                case ExpressionNode::Unary::Neg:
                  reduced = new LiteralNode(-term->value());
                  break;
                  
                case ExpressionNode::Unary::Not:
                  reduced = new LiteralNode(!term->value());
                  break;
              }
              
              reduced->setParent(expression->parent());
              delete expression;
              node = reduced;
            }
          }
          // Simplify binary operations
          else if (expression->isBinary())
          {
            lang::Node*& lhsNode = expression->children()[0];
            lang::Node*& rhsNode = expression->children()[1];
            
            pass->pass(lhsNode);
            pass->pass(rhsNode);
            
            // Simplify Literal <-> Literal expressions
            if (lhsNode->which() == LITERAL_NODE &&
                rhsNode->which() == LITERAL_NODE)
            {
              LiteralNode* lhs = static_cast<LiteralNode*>(lhsNode);
              LiteralNode* rhs = static_cast<LiteralNode*>(rhsNode);
              LiteralNode* reduced = nullptr;
              
              switch (expression->binaryOperation())
              {
                case ExpressionNode::Binary::Add:
                  reduced = new LiteralNode(lhs->value() + rhs->value());
                  break;
                  
                case ExpressionNode::Binary::Sub:
                  reduced = new LiteralNode(lhs->value() - rhs->value());
                  break;
                  
                case ExpressionNode::Binary::Mul:
                  reduced = new LiteralNode(lhs->value() * rhs->value());
                  break;
                  
                case ExpressionNode::Binary::Div:
                  reduced = new LiteralNode(lhs->value() / rhs->value());
                  break;
                  
                case ExpressionNode::Binary::And:
                  reduced = new LiteralNode(lhs->value() && rhs->value());
                  break;
                  
                case ExpressionNode::Binary::Or:
                  reduced = new LiteralNode(lhs->value() || rhs->value());
                  break;
                  
                case ExpressionNode::Binary::Lt:
                  reduced = new LiteralNode(lhs->value() < rhs->value());
                  break;
                  
                case ExpressionNode::Binary::Lte:
                  reduced = new LiteralNode(lhs->value() <= rhs->value());
                  break;
                  
                case ExpressionNode::Binary::Gt:
                  reduced = new LiteralNode(lhs->value() > rhs->value());
                  break;
                  
                case ExpressionNode::Binary::Gte:
                  reduced = new LiteralNode(lhs->value() >= rhs->value());
                  break;
                  
                case ExpressionNode::Binary::Eq:
                  reduced = new LiteralNode(lhs->value() == rhs->value());
                  break;
                  
                case ExpressionNode::Binary::Neq:
                  reduced = new LiteralNode(lhs->value() != rhs->value());
                  break;
              }
              
              reduced->setParent(expression->parent());
              delete expression;
              node = reduced;
            }
            // Simplify some Literal <-> Non-literal expressions
            // 1 * Non-literal -> Non-literal
            // 0 + Non-literal -> Non-literal
            // 0 - Non-literal -> - Non-literal
            else if (lhsNode->which() == LITERAL_NODE)
            {
              // don't work with lhsNode / rhsNode references to expression->children() elements
              // as we may remove some elements off this vector
              lang::Node* lhs = lhsNode;
              lang::Node* rhs = rhsNode;
              
              core::Literal const&  lhsVal = static_cast<LiteralNode*>(lhs)->value();
              bool one = false;
              bool zero = false;
              
              if ((lhsVal.isFloating() && lhsVal.floating() == 1.0) || (lhsVal.isInteger() && lhsVal.integer() == 1))
              {
                one = true;
              }
              else if ((lhsVal.isFloating() && lhsVal.floating() == 0.0) || (lhsVal.isInteger() && lhsVal.integer() == 0))
              {
                zero = true;
              }
              
              if ((zero && expression->binaryOperation() == ExpressionNode::Binary::Add) ||
                  (one && expression->binaryOperation() == ExpressionNode::Binary::Mul))
              {
                expression->removeChild(rhs);
                rhs->setParent(expression->parent());
                node = rhs;
                
                delete expression;
              }
              else if (zero && expression->binaryOperation() == ExpressionNode::Binary::Sub)
              {
                  expression->removeChild(rhs);
                  ExpressionNode* neg = new ExpressionNode(ExpressionNode::Unary::Neg, static_cast<ExpressionNode*>(rhs));
                  neg->setParent(expression->parent());
                  node = neg;
                  
                  delete expression;
              }
            }
            // Simplify some Non-literal <-> Literal expressions
            // Non-literal * 1 -> Non-literal
            // Non-literal / 1 -> Non-literal
            // Non-literal + 0 -> Non-literal
            // Non-literal - 0 -> Non-literal
            else if (rhsNode->which() == LITERAL_NODE)
            {
              // don't work with lhsNode / rhsNode references to expression->children() elements
              // as we may remove some elements off this vector
              lang::Node* lhs = lhsNode;
              lang::Node* rhs = rhsNode;
              
              core::Literal const& rhsVal = static_cast<LiteralNode*>(rhs)->value();
              bool one = false;
              bool zero = false;
              
              if ((rhsVal.isFloating() && rhsVal.floating() == 1.0) || (rhsVal.isInteger() && rhsVal.integer() == 1))
              {
                one = true;
              }
              else if ((rhsVal.isFloating() && rhsVal.floating() == 0.0) || (rhsVal.isInteger() && rhsVal.integer() == 0))
              {
                zero = true;
              }
              
              if ((expression->binaryOperation() == ExpressionNode::Binary::Add && zero) ||
                  (expression->binaryOperation() == ExpressionNode::Binary::Sub && zero) ||
                  (expression->binaryOperation() == ExpressionNode::Binary::Mul && one) ||
                  (expression->binaryOperation() == ExpressionNode::Binary::Div && one))
              {
                expression->removeChild(lhs);
                lhs->setParent(expression->parent());
                node = lhs;
                
                delete expression;
              }
            }
          }
        }
      );
      
      return pass;
    }
  }
}
