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
          
          std::cout << "(variable) " << variable->value().name() << std::endl;
        }
      );
      
      pass.addOperator<FUNCTION_CALL_NODE, FunctionCallNode>(
      [](lang::TreePass* pass, lang::Node*& node, FunctionCallNode* call)
        {
          for (int i = 0; i < indent; ++i)
          {
            std::cout << "  ";
          }
          
          std::cout << "[function call: " << call->value().name() << std::endl;
          
          ++indent;
          for (lang::Node*& child : call->children())
          {
            pass->pass(child);
          }
          --indent;
          
          for (int i = 0; i < indent; ++i)
          {
            std::cout << "  ";
          }
          
          std::cout << "]" << std::endl;
        }
      );
      
      pass.addOperator<CAST_NODE, CastNode>(
        [](lang::TreePass* pass, lang::Node*& node, CastNode* cast)
        {
          for (int i = 0; i < indent; ++i)
          {
            std::cout << "  ";
          }
          
          std::cout << "[cast]" << std::endl;
          ++indent;
          pass->pass(cast->children()[0]);
          --indent;
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
          
          std::cout << "{variable declaration : `" << decl->variable().name() << "'" << std::endl;
          
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
          
          std::cout << "{variable assignment : `" << assign->variable().name() << "'" << std::endl;
          
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

      pass.addOperator<RETURN_STATEMENT_NODE, ReturnStatementNode>(
        [](lang::TreePass* pass, lang::Node*& node, ReturnStatementNode* stat)
        {
          for (int i = 0; i < indent; ++i)
          {
            std::cout << "  ";
          }
         
          std::cout << "{return statement";
         
          if (stat->children().size())
          {
            std::cout << ": " << std::endl;
            ++indent;
            pass->pass(stat->children()[0]);
            --indent;
            for (int i = 0; i < indent; ++i)
            {
              std::cout << "  ";
            }
            std::cout << "}" << std::endl;
          }
          else
          {
            std::cout << "}" << std::endl;
          }
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
      
      pass.addOperator<FUNCTION_DECLARATION_NODE, FunctionDeclarationNode>(
      [](lang::TreePass* pass, lang::Node*& node, FunctionDeclarationNode* decl)
        {
          for (int i = 0; i < indent; ++i)
          {
            std::cout << "  ";
          }
          
          std::cout << "{function_declaration : `" << decl->function().name() << "(";
          for (auto arg = decl->function().prototype().args().begin();
               arg != decl->function().prototype().args().end();
               ++arg)
          {
            std::cout << arg->name();
            if (++arg != decl->function().prototype().args().end())
            {
              std::cout << ", ";
            }
            --arg;
          }
          std::cout << ")'" << std::endl;
          
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
      
      pass.addOperator<PROGRAM_NODE, ProgramNode>(
        [](lang::TreePass* pass, lang::Node*& node, ProgramNode* program)
        {
          for (int i = 0; i < indent; ++i)
          {
            std::cout << "  ";
          }
          
          std::cout << "{program :" << std::endl;
          
          ++indent;
          for (auto it = program->children().begin(); it != program->children().end(); ++it)
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
  }
}
