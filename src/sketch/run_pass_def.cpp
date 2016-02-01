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

            auto variable_ref = [](lang::Node * node, core::Variable const& variable) -> core::Variable &
            {
                return node->scopeLayer()->findRef(variable.name())->variable();
            };

            //// LiteralNode ////
            {
                auto rule = [&](lang::TreePass* pass, lang::Node*& node, LiteralNode* literal)
                {
                    core::Literal const& val = literal->value();
                    // stack_push(pass, val.value());

                    pass->storage() = val.value();
                };

                pass.addOperator<LITERAL_NODE, LiteralNode>(rule);
            }

            //// VariableNode ////
            {
                auto rule = [&](lang::TreePass* pass, lang::Node*& node, VariableNode* variable)
                {
                    core::Variable& var = variable_ref(node, variable->value());

                    pass->storage() = var.value();
                };

                pass.addOperator<VARIABLE_NODE, VariableNode>(rule);
            }

            //// FunctionCallNode ////
            {
                auto rule = [&](lang::TreePass* pass, lang::Node*& node, FunctionCallNode* call)
                {
                    // Retrieve the function body AST
                    lang::Node* fun_body = call->value().node();

                    // Set up argument values
                    std::vector<lang::Node*>::iterator it = call->children().begin();
                    for(core::Variable const& arg : call->value().prototype().args())
                    {
                        // Compute expression result
                        pass->pass(*it++);

                        // Retrieve the reference to the argument
                        core::Variable& var = variable_ref(fun_body, arg);
                        var.setValue(pass->storage());
                    }

                    // Pass over function body
                    pass->pass(fun_body->children()[0]);
                };

                pass.addOperator<FUNCTION_CALL_NODE, FunctionCallNode>(rule);
            }

            //// CallbackCallNode ////
            {
                auto rule = [&](lang::TreePass* pass, lang::Node*& node, CallbackCallNode* call)
                {
                    // Build argument vector
                    std::vector<core::Some> args;

                    for(lang::Node* n : call->children())
                    {
                        // Compute expression result
                        pass->pass(n);
                        args.push_back(pass->storage());
                    }

                    // Invoke callback
                    pass->storage() = call->value().call(args);
                };

                pass.addOperator<CALLBACK_CALL_NODE, CallbackCallNode>(rule);
            }

            //// CastNode ////
            {
                auto rule = [&](lang::TreePass* pass, lang::Node*& node, CastNode* cast)
                {
                    pass->pass(node->children()[0]);
                    core::Literal value = pass->storage();

                    if(cast->to() == core::Type::Opaque || cast->to() == core::Type::Nil) node->error("invalid cast");

                    pass->storage() = value.cast(cast->to());
                };

                pass.addOperator<CAST_NODE, CastNode>(rule);
            }

            //// ExpressionNode ////
            {
                auto rule = [&](lang::TreePass* pass, lang::Node*& node, ExpressionNode* expression)
                {
                    if(expression->isUnary())
                    {
                        pass->pass(expression->children()[0]);
                        core::Literal value = pass->storage();
                        core::Some result;

                        if(expression->unaryOperation() == ExpressionNode::Unary::Neg)
                        {
                            result = (-value).value();
                            if(!result.valid()) node->error("negating something not a number");
                        }
                        else if(expression->unaryOperation() == ExpressionNode::Unary::Not)
                        {
                            result = (!value).value();
                            if(!result.valid()) node->error("not'ing something not a boolean");
                        }

                        pass->storage() = result;
                    }
                    else if(expression->isBinary())
                    {
                        pass->pass(expression->children()[0]);
                        core::Literal lhs = pass->storage();

                        pass->pass(expression->children()[1]);
                        core::Literal rhs = pass->storage();

                        core::Some result;

                        switch(expression->binaryOperation())
                        {
                            case ExpressionNode::Binary::Add:
                                result = (lhs + rhs).value();
                                if(!result.valid()) node->error("not adding numbers");
                                break;

                            case ExpressionNode::Binary::Sub:
                                result = (lhs - rhs).value();
                                if(!result.valid()) node->error("not subtracting numbers");
                                break;

                            case ExpressionNode::Binary::Mul:
                                result = (lhs * rhs).value();
                                if(!result.valid()) node->error("not adding numbers");
                                break;

                            case ExpressionNode::Binary::Div:
                                result = (lhs / rhs).value();
                                if(!result.valid()) node->error("not dividing numbers");
                                break;

                            case ExpressionNode::Binary::Lt:
                                result = (lhs < rhs).value();
                                if(!result.valid()) node->error("not comparing numbers");
                                break;

                            case ExpressionNode::Binary::Lte:
                                result = (lhs <= rhs).value();
                                if(!result.valid()) node->error("not comparing numbers");
                                break;

                            case ExpressionNode::Binary::Gt:
                                result = (lhs > rhs).value();
                                if(!result.valid()) node->error("not comparing numbers");
                                break;

                            case ExpressionNode::Binary::Gte:
                                result = (lhs >= rhs).value();
                                if(!result.valid()) node->error("not comparing numbers");
                                break;

                            case ExpressionNode::Binary::Eq:
                                result = (lhs == rhs).value();
                                if(!result.valid()) node->error("not comparing numbers");
                                break;

                            case ExpressionNode::Binary::Neq:
                                result = (lhs != rhs).value();
                                if(!result.valid()) node->error("not comparing numbers");
                                break;

                            case ExpressionNode::Binary::And:
                                result = (lhs && rhs).value();
                                if(!result.valid()) node->error("not and'ing booleans");
                                break;

                            case ExpressionNode::Binary::Or:
                                result = (lhs || rhs).value();
                                if(!result.valid()) node->error("not or'ing booleans");
                                break;
                        }

                        pass->storage() = result;
                    }
                };

                pass.addOperator<EXPRESSION_NODE, ExpressionNode>(rule);
            }

            //// VariableDeclNode ////
            {
                auto rule = [&](lang::TreePass* pass, lang::Node*& node, VariableDeclNode* decl)
                {
                    // Do something only if variable has initializer
                    if(decl->children().size())
                    {
                        // Compute initializer value
                        pass->pass(decl->children()[0]);

                        // Store it into the appriopriate scope
                        core::Variable& var = variable_ref(node, decl->variable());
                        var.setValue(pass->storage());
                    }
                };

                pass.addOperator<VARIABLE_DECL_NODE, VariableDeclNode>(rule);
            }

            //// VariableAssignNode ////
            {
                auto rule = [&](lang::TreePass* pass, lang::Node*& node, VariableAssignNode* assign)
                {
                    // Compute expression value
                    pass->pass(assign->children()[0]);

                    // Store it into the appriopriate scope
                    core::Variable& var = variable_ref(node, assign->variable());
                    var.setValue(pass->storage());
                };

                pass.addOperator<VARIABLE_ASSIGN_NODE, VariableAssignNode>(rule);
            }

            //// ReturnStatementNode ////
            {
                auto rule = [&](lang::TreePass* pass, lang::Node*& node, ReturnStatementNode* stat)
                {
                    // Just push the return value if any
                    if(stat->children().size())
                    {
                        pass->pass(stat->children()[0]);
                    }
                };

                pass.addOperator<RETURN_STATEMENT_NODE, ReturnStatementNode>(rule);
            }

            //// StatementBlockNode ////
            {
                auto rule = [&](lang::TreePass* pass, lang::Node*& node, StatementBlockNode* block)
                {
                    for(auto it = block->children().begin(); it != block->children().end(); ++it)
                    {
                        pass->pass(*it);
                    }
                };

                pass.addOperator<STATEMENT_BLOCK_NODE, StatementBlockNode>(rule);
            }

            //// FunctionDeclarationNode ////
            {
                auto rule = [&](lang::TreePass* pass, lang::Node*& node, FunctionDeclarationNode* decl)
                {
                    /* don't do anything for function declarations */
                };

                pass.addOperator<FUNCTION_DECLARATION_NODE, FunctionDeclarationNode>(rule);
            }

            //// ProgramNode ////
            {
                auto rule = [&](lang::TreePass* pass, lang::Node*& node, ProgramNode* program)
                {
                    for(auto it = program->children().begin(); it != program->children().end(); ++it)
                    {
                        pass->pass(*it);
                    }
                };

                pass.addOperator<PROGRAM_NODE, ProgramNode>(rule);
            }

            return pass;
        }
    }
}
