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
    lang::TreePass getTypecheckPass()
    {
      lang::TreePass typeCheck;
      
      typeCheck.addOperator<LITERAL_NODE, LiteralNode>(
        [](lang::TreePass* pass, lang::Node*& node, LiteralNode* literal)
        {
          pass->storage() = literal->value().type();
        }
      );
      
      typeCheck.addOperator<CAST_NODE, CastNode>(
        [](lang::TreePass* pass, lang::Node*& node, CastNode* cast)
        {
          pass->pass(cast->children()[0]);
          pass->storage() = cast->to();
        }
      );
      
      typeCheck.addOperator<VARIABLE_NODE, VariableNode>(
        [](lang::TreePass* pass, lang::Node*& node, VariableNode* variable)
        {
          pass->storage() = variable->value().type();
        }
      );
      
      typeCheck.addOperator<FUNCTION_CALL_NODE, FunctionCallNode>(
        [](lang::TreePass* pass, lang::Node*& node, FunctionCallNode* call)
        {
          core::Prototype const& prototype = call->value().prototype();
          
          if (prototype.args().size() != call->children().size())
          {
            node->error("wrong number of arguments in function call");
          }
          else
          {
            auto arg_it = prototype.args().begin();
            for (lang::Node*& child : call->children())
            {
              pass->pass(child);
              core::Type rtp = pass->storage().cast<core::Type>();
              if (arg_it->type() != rtp)
              {
                child->error("invalid implicit type conversion");
                break;
              }
              ++arg_it;
            }
          }
          
          pass->storage() = prototype.ret();
        }
      );
      
      typeCheck.addOperator<EXPRESSION_NODE, ExpressionNode>(
        [](lang::TreePass* pass, lang::Node*& node, ExpressionNode* expression)
        {
          if (expression->isUnary())
          {
            pass->pass(expression->children()[0]);
          }
          else if (expression->isBinary())
          {
            pass->pass(expression->children()[0]);
            core::Type ltp = pass->storage().cast<core::Type>();
            pass->pass(expression->children()[1]);
            core::Type rtp = pass->storage().cast<core::Type>();
            
            if (ltp != rtp)
            {
              expression->error("invalid implicit type conversion");
            }
          }
        }
      );
      
      typeCheck.addOperator<VARIABLE_DECL_NODE, VariableDeclNode>(
        [](lang::TreePass* pass, lang::Node*& node, VariableDeclNode* decl)
        {
          if (decl->children().size())
          {
            core::Type ltp = decl->variable().type();
            pass->pass(decl->children()[0]);
            core::Type rtp = pass->storage().cast<core::Type>();
            
            if (ltp != rtp)
            {
              decl->error("invalid implicit type conversion");
            }
          }
        }
      );
      
      typeCheck.addOperator<VARIABLE_ASSIGN_NODE, VariableAssignNode>(
       [](lang::TreePass* pass, lang::Node*& node, VariableAssignNode* assign)
       {
         core::Type ltp = assign->variable().type();
         pass->pass(assign->children()[0]);
         core::Type rtp = pass->storage().cast<core::Type>();
         
         if (ltp != rtp)
         {
           assign->error("invalid implicit type conversion");
         }
       }
      );
      
      typeCheck.addOperator<RETURN_STATEMENT_NODE, ReturnStatementNode>(
        [](lang::TreePass* pass, lang::Node*& node, ReturnStatementNode* stat)
        {
          core::Symbol self;
          if (!node->scopeLayer()->findInScope("self", &self))
            stat->error("return statement not in function !?");

          core::Type ltp = self.function().prototype().ret();
          
          core::Type rtp = core::Type::Nil;
          if (stat->children().size())
          {
            pass->pass(stat->children()[0]);
            rtp = pass->storage().cast<core::Type>();
          }
          
          if (ltp != rtp)
          {
            stat->error("invalid implicit type conversion");
          }
        }
      );
      
      return typeCheck;
    }
  }
}
