#include "sketch/pass_def.hpp"
#include "sketch/ast.hpp"
#include "lang/parser.hpp"
#include "core/symbol.hpp"
#include "core/type.hpp"
#include "core/function.hpp"

#include <iostream>
#include <stack>

namespace gpse
{
  namespace sketch
  {
    lang::TreePass getRunPass()
    {
      lang::TreePass pass;

      auto variable_ref = [](lang::Node* node, core::Variable const& variable) -> core::Variable&
      {
        return node->scopeLayer()->findRef(variable.name())->variable();
      };
      
      pass.addOperator<LITERAL_NODE, LiteralNode>(
        [&](lang::TreePass* pass, lang::Node*& node, LiteralNode* literal)
        {
          core::Literal const& val = literal->value();
          // stack_push(pass, val.value());

          pass->storage() = val.value();
        }
      );
      
      pass.addOperator<VARIABLE_NODE, VariableNode>(
        [&](lang::TreePass* pass, lang::Node*& node, VariableNode* variable)
        {
          core::Variable& var = variable_ref(node, variable->value());
          // stack_push(pass, var.value());

          pass->storage() = var.value();
        }
      );
      
      pass.addOperator<FUNCTION_CALL_NODE, FunctionCallNode>(
        [&](lang::TreePass* pass, lang::Node*& node, FunctionCallNode* call)
        {
          // Retrieve the function body AST
          lang::Node* fun_body = call->value().node();

          // Set up argument values
          std::vector<lang::Node*>::iterator it = call->children().begin();
          for (core::Variable const& arg : call->value().prototype().args())
          {
            // Compute expression result
            pass->pass(*it++);

            // Retrieve the reference to the argument
            core::Variable& var = variable_ref(fun_body, arg);
            var.setValue(pass->storage());
          }

          // Pass over function body
          pass->pass(fun_body->children()[0]);
        }
      );
      
      pass.addOperator<CAST_NODE, CastNode>(
        [&](lang::TreePass* pass, lang::Node*& node, CastNode* cast)
        {
          /*for (int i = 0; i < indent; ++i)
          {
            std::cout << "  ";
          }
          
          std::cout << "[cast]" << std::endl;
          ++indent;
          pass->pass(cast->children()[0]);
          --indent;*/
        }
      );
      
      pass.addOperator<EXPRESSION_NODE, ExpressionNode>(
        [&](lang::TreePass* pass, lang::Node*& node, ExpressionNode* expression)
        {
          if (expression->isUnary())
          {
            pass->pass(expression->children()[0]);
            core::Some value = pass->storage(); // stack_pop(pass);
            core::Some result;

            if (expression->unaryOperation() == ExpressionNode::Unary::Neg)
            {
              if (value.is<int>())
                result = -value.cast<int>();
              else if (value.is<float>())
                result = -value.cast<float>();
              else
                node->error("negating something not a number");
            }
            else if (expression->unaryOperation() == ExpressionNode::Unary::Not)
            {
              if (value.is<bool>())
                result = !value.cast<bool>();
              else
                node->error("not'ing something not a boolean");
            }

            pass->storage() = result;
          }
          else if (expression->isBinary())
          {
            pass->pass(expression->children()[0]);
            core::Some lhs = pass->storage();

            pass->pass(expression->children()[1]);
            core::Some rhs = pass->storage();

            core::Some result;

            switch (expression->binaryOperation())
            {
              case ExpressionNode::Binary::Add:
                if (lhs.is<int>())
                  result = lhs.cast<int>() + rhs.cast<int>();
                else if (lhs.is<float>())
                  result = lhs.cast<float>() + rhs.cast<float>();
                break;
                
              case ExpressionNode::Binary::Sub:
                if (lhs.is<int>())
                  result = lhs.cast<int>() - rhs.cast<int>();
                else if (lhs.is<float>())
                  result = lhs.cast<float>() - rhs.cast<float>();
                else
                  node->error("adding something not a number");
                break;
                
              case ExpressionNode::Binary::Mul:
                if (lhs.is<int>())
                  result = lhs.cast<int>() * rhs.cast<int>();
                else if (lhs.is<float>())
                  result = lhs.cast<float>() * rhs.cast<float>();
                else
                  node->error("multiplying something not a number");
                break;
                
              case ExpressionNode::Binary::Div:
                if (lhs.is<int>())
                  result = lhs.cast<int>() / rhs.cast<int>();
                else if (lhs.is<float>())
                  result = lhs.cast<float>() / rhs.cast<float>();
                else
                  node->error("dividing something not a number");
                break;
                
              case ExpressionNode::Binary::Lt:
                if (lhs.is<int>())
                  result = lhs.cast<int>() < rhs.cast<int>();
                else if (lhs.is<float>())
                  result = lhs.cast<float>() < rhs.cast<float>();
                else
                  node->error("lt'ing something not a number");
                break;
                
              case ExpressionNode::Binary::Lte:
                if (lhs.is<int>())
                  result = lhs.cast<int>() <= rhs.cast<int>();
                else if (lhs.is<float>())
                  result = lhs.cast<float>() <= rhs.cast<float>();
                else
                  node->error("lte'ing something not a number");
                break;
                
              case ExpressionNode::Binary::Gt:
                if (lhs.is<int>())
                  result = lhs.cast<int>() > rhs.cast<int>();
                else if (lhs.is<float>())
                  result = lhs.cast<float>() > rhs.cast<float>();
                else
                  node->error("gt'ing something not a number");
                break;
                
              case ExpressionNode::Binary::Gte:
                if (lhs.is<int>())
                  result = lhs.cast<int>() >= rhs.cast<int>();
                else if (lhs.is<float>())
                  result = lhs.cast<float>() >= rhs.cast<float>();
                else
                  node->error("gte'ing something not a number");
                break;
                
              case ExpressionNode::Binary::Eq:
                if (lhs.is<int>())
                  result = lhs.cast<int>() == rhs.cast<int>();
                else if (lhs.is<float>())
                  result = lhs.cast<float>() == rhs.cast<float>();
                else
                  node->error("eq'ing something not a number");
                break;
                
              case ExpressionNode::Binary::Neq:
                if (lhs.is<int>())
                  result = lhs.cast<int>() != rhs.cast<int>();
                else if (lhs.is<float>())
                  result = lhs.cast<float>() != rhs.cast<float>();
                else
                  node->error("neq'ing something not a number");
                break;
                
              case ExpressionNode::Binary::And:
                if (lhs.is<bool>())
                  result = lhs.cast<bool>() && rhs.cast<bool>();
                else
                  node->error("and'ing something not a boolean");
                break;
                
              case ExpressionNode::Binary::Or:
                if (lhs.is<bool>())
                  result = lhs.cast<bool>() || rhs.cast<bool>();
                else
                  node->error("or'ing something not a boolean");
                break;
            }

            pass->storage() = result;
          }
        }
      );
      
      pass.addOperator<VARIABLE_DECL_NODE, VariableDeclNode>(
        [&](lang::TreePass* pass, lang::Node*& node, VariableDeclNode* decl)
        {
          // Do something only if variable has initializer
          if (decl->children().size())
          {
            // Compute initializer value
            pass->pass(decl->children()[0]);

            // Store it into the appriopriate scope
            core::Variable& var = variable_ref(node, decl->variable());
            var.setValue(pass->storage());
          }
        }
      );
      
      pass.addOperator<VARIABLE_ASSIGN_NODE, VariableAssignNode>(
        [&](lang::TreePass* pass, lang::Node*& node, VariableAssignNode* assign)
        {
          // Compute expression value
          pass->pass(assign->children()[0]);

          // Store it into the appriopriate scope
          core::Variable& var = variable_ref(node, assign->variable());
          var.setValue(pass->storage());
        }
      );
      
      pass.addOperator<RETURN_STATEMENT_NODE, ReturnStatementNode>(
        [&](lang::TreePass* pass, lang::Node*& node, ReturnStatementNode* stat)
        {
          // Just push the return value if any
          if (stat->children().size())
          {
            pass->pass(stat->children()[0]);
          }
        }
      );
      
      pass.addOperator<STATEMENT_BLOCK_NODE, StatementBlockNode>(
        [&](lang::TreePass* pass, lang::Node*& node, StatementBlockNode* block)
        {
          for (auto it = block->children().begin(); it != block->children().end(); ++it)
          {
            pass->pass(*it);
          }
        }
      );
      
      pass.addOperator<FUNCTION_DECLARATION_NODE, FunctionDeclarationNode>(
        [&](lang::TreePass* pass, lang::Node*& node, FunctionDeclarationNode* decl)
        {
          /* don't do anything for function declarations */
        }
      );
      
      pass.addOperator<PROGRAM_NODE, ProgramNode>(
        [&](lang::TreePass* pass, lang::Node*& node, ProgramNode* program)
        {
          for (auto it = program->children().begin(); it != program->children().end(); ++it)
          {
            pass->pass(*it);
          }
        }
      );
      
      return pass;
    }
  }
}
