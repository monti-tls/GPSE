#include "sketch/pass_def.hpp"
#include "sketch/ast.hpp"

#include <iostream>

namespace gpse
{
  namespace sketch
  {
    lang::TreePass getPrinterPass()
    {
      static int indent = 0;
      
      lang::TreePass pass;
      
      pass.addOperator<LITERAL_NODE, LiteralNode>(
        [](lang::TreePass* pass, lang::Node*& node, LiteralNode* literal)
        {
          for (int i = 0; i < indent; ++i)
          {
            std::cout << "  ";
          }
          core::Literal const& val = literal->value();
          if (val.isNil())
          {
            std::cout << "(nil)";
          }
          else if (val.isBoolean())
          {
            std::cout << "(bool) " << (val.boolean() ? "true" : "false");
          }
          else if (val.isInteger())
          {
            std::cout << "(int) " << val.integer();
          }
          else if (val.isFloating())
          {
            std::cout << "(float) " << val.floating();
          }
          else if (val.isString())
          {
            std::cout << "(string) \"" << val.string() << '"';
          }
          std::cout << std::endl;
        }
      );
      
      pass.addOperator<VARIABLE_NODE, VariableNode>(
        [](lang::TreePass* pass, lang::Node*& node, VariableNode* variable)
        {
          for (int i = 0; i < indent; ++i)
          {
            std::cout << "  ";
          }
          
          std::cout << "(variable) " << variable->value() << std::endl;
        }
      );
      
      pass.addOperator<EXPRESSION_NODE, ExpressionNode>(
        [](lang::TreePass* pass, lang::Node*& node, ExpressionNode* expression)
        {
          for (int i = 0; i < indent; ++i)
          {
            std::cout << "  ";
          }
          
          if (expression->isUnary())
          {
            if (expression->unaryOperation() == ExpressionNode::Unary::Neg)
            {
              std::cout << "[neg]" << std::endl;
            }
            else if (expression->unaryOperation() == ExpressionNode::Unary::Not)
            {
              std::cout << "[not]" << std::endl;
            }
            
            ++indent;
            pass->pass(expression->children()[0]);
            --indent;
          }
          else if (expression->isBinary())
          {
            switch (expression->binaryOperation())
            {
              case ExpressionNode::Binary::Add:
                std::cout << "[add]" << std::endl;
                break;
                
              case ExpressionNode::Binary::Sub:
                std::cout << "[sub]" << std::endl;
                break;
                
              case ExpressionNode::Binary::Mul:
                std::cout << "[mul]" << std::endl;
                break;
                
              case ExpressionNode::Binary::Div:
                std::cout << "[div]" << std::endl;
                break;
                
              case ExpressionNode::Binary::Lt:
                std::cout << "[lt]" << std::endl;
                break;
                
              case ExpressionNode::Binary::Lte:
                std::cout << "[lte]" << std::endl;
                break;
                
              case ExpressionNode::Binary::Gt:
                std::cout << "[gt]" << std::endl;
                break;
                
              case ExpressionNode::Binary::Gte:
                std::cout << "[gte]" << std::endl;
                break;
                
              case ExpressionNode::Binary::Eq:
                std::cout << "[eq]" << std::endl;
                break;
                
              case ExpressionNode::Binary::Neq:
                std::cout << "[neq]" << std::endl;
                break;
                
              case ExpressionNode::Binary::And:
                std::cout << "[and]" << std::endl;
                break;
                
              case ExpressionNode::Binary::Or:
                std::cout << "[or]" << std::endl;
                break;
            }
            
            ++indent;
            pass->pass(expression->children()[0]);
            pass->pass(expression->children()[1]);
            --indent;
          }
        }
      );
      
      pass.addOperator<VARIABLE_DECL_NODE, VariableDeclNode>(
        [](lang::TreePass* pass, lang::Node*& node, VariableDeclNode* decl)
        {
          for (int i = 0; i < indent; ++i)
          {
            std::cout << "  ";
          }
          
          std::cout << "{variable declaration : `" << decl->name() << "'" << std::endl;
          
          if (decl->children().size())
          {
            ++indent;
            pass->pass(decl->children()[0]);
            --indent;
          }
          
          for (int i = 0; i < indent; ++i)
          {
            std::cout << "  ";
          }
          
          std::cout << "}" << std::endl;
        }
      );
      
      pass.addOperator<VARIABLE_ASSIGN_NODE, VariableAssignNode>(
        [](lang::TreePass* pass, lang::Node*& node, VariableAssignNode* assign)
        {
          for (int i = 0; i < indent; ++i)
          {
            std::cout << "  ";
          }
          
          std::cout << "{variable assignment : `" << assign->name() << "'" << std::endl;
          
          ++indent;
          pass->pass(assign->children()[0]);
          --indent;
          
          for (int i = 0; i < indent; ++i)
          {
            std::cout << "  ";
          }
          
          std::cout << "}" << std::endl;
        }
      );
      
      pass.addOperator<STATEMENT_BLOCK_NODE, StatementBlockNode>(
        [](lang::TreePass* pass, lang::Node*& node, StatementBlockNode* block)
        {
          for (int i = 0; i < indent; ++i)
          {
            std::cout << "  ";
          }
          
          std::cout << "{statement_block :" << std::endl;
          
          ++indent;
          for (auto it = block->children().begin(); it != block->children().end(); ++it)
          {
            pass->pass(*it);
          }
          --indent;
          
          for (int i = 0; i < indent; ++i)
          {
            std::cout << "  ";
          }
          
          std::cout << "}" << std::endl;
        }
      );
      
      return pass;
    }

    lang::TreePass getLiteralReductionPass()
    {
      lang::TreePass pass;
      
      pass.addOperator<LITERAL_NODE, LiteralNode>(
        [](lang::TreePass* pass, lang::Node*& node, LiteralNode* literal)
        {
          // a literal node is already reduced
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
