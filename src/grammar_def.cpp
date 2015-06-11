#include "sketch/grammar_def.hpp"
#include "sketch/rule_def.hpp"
#include "sketch/ast.hpp"
#include "core/symbol.hpp"
#include "core/scope.hpp"

namespace gpse
{
  namespace sketch
  {
    void setupParser(lang::Parser& parser)
    { 
      parser.grammars()["atom_term"] = lang::Grammar(
        [](lang::Parser* p) -> bool
        {
          return p->seek().which == VARIABLENAME
              || p->seek().which == FUNCTIONNAME
              || p->seek().which == NUMBER;
        },
        
        [](lang::Parser* p) -> lang::Node*
        {
          lang::Token tok;
          
          if (p->seek().which == NUMBER)
          {
            p->eat(NUMBER, tok);
            LiteralNode* node = new LiteralNode(tok.value);
            node->setToken(tok);
            return node;
          }
          else if (p->seek().which == STRING)
          {
            p->eat(STRING, tok);
            LiteralNode* node = new LiteralNode(tok.value);
            node->setToken(tok);
            return node;
          }
          else if (p->seek().which == VARIABLENAME)
          {
            p->eat(VARIABLENAME, tok);
            core::Variable variable = tok.value.cast<core::Variable>();
            VariableNode* node = new VariableNode(variable);
            node->setToken(tok);
            return node;
          }
          else if (p->seek().which == FUNCTIONNAME)
          {
            p->eat(FUNCTIONNAME, tok);
            lang::Token ptok = tok;
            
            core::Function function = tok.value.cast<core::Function>();
            FunctionCallNode* node = new FunctionCallNode(function);
            
            if (!p->eat(LPAR, tok))
            {
              p->error("expected `)'");
              delete node;
              return nullptr;
            }
            
            if (p->seek().which != RPAR)
            {
              do
              {
                if (p->seek() == COMMA)
                {
                  p->eat(COMMA, tok);
                }
                
                lang::Node* arg = p->parse("expression");
                if (!arg)
                {
                  delete node;
                  return nullptr;
                }
                
                node->addChild(arg);
              } while (p->seek() == COMMA);
            }
            
            if (!p->eat(RPAR, tok))
            {
              p->error("expected `)'");
              delete node;
              return nullptr;
            }
            
            return node;
          }
          else if (p->seek().which == LPAR)
          {
            p->eat(LPAR, tok);
            
            if (p->seek().which == TYPENAME)
            {
              p->eat(TYPENAME, tok);
              lang::Token ptok = tok;
              if (!p->eat(RPAR, tok))
              {
                p->error("expected `)'");
                return nullptr;
              }
              ExpressionNode* expr = p->parse<ExpressionNode>("expression");
              if (!expr)
              {
                return nullptr;
              }
              CastNode* node = new CastNode(ptok.value.cast<core::Type>(), expr);
              node->setToken(ptok);
              return node;
            }
            else
            {
              ExpressionNode* expr = p->parse<ExpressionNode>("expression");
              if (!p->eat(RPAR, tok))
              {
                p->error("expected `)'");
              }
              return expr;
            }
          }
          else if (p->seek().which == IDENT)
          {
            p->error("invalid use of undeclared identifier `" + p->seek().value.cast<std::string>() + "'");
            return nullptr;
          }
          else if (p->seek().which == K_TRUE)
          {
            p->eat(K_TRUE, tok);
            LiteralNode* node = new LiteralNode(true);
            node->setToken(tok);
            return node;
          }
          else if (p->seek().which == K_FALSE)
          {
            p->eat(K_FALSE, tok);
            LiteralNode* node = new LiteralNode(false);
            node->setToken(tok);
            return node;
          }
          else
          {
            p->error("expected expression");
            return nullptr;
          }
        }
      );
      
      parser.grammars()["unary_not_term"] = lang::Grammar(
        [](lang::Parser* p) -> bool
        {
          return p->seek().which == NOT ||
                 p->predicate("atom");
        },
        
        [](lang::Parser* p) -> lang::Node*
        {
          lang::Token tok;
          
          bool is = false;
          while (p->seek().which == NOT)
          {
            p->eat(NOT, tok);
            is = !is;
          }
          
          lang::Node* atom = p->parse("atom_term");
          
          if (!is || !atom)
          {
            return atom;
          }
          else
          {
            ExpressionNode* node = new ExpressionNode(ExpressionNode::Unary::Not, atom);
            node->setToken(tok);
            return node;
          }
        }
      );
      
      parser.grammars()["unary_neg_term"] = lang::Grammar(
        [](lang::Parser* p) -> bool
        {
          return p->seek().which == MINUS ||
                 p->predicate("unary_not_term");
        },
        
        [](lang::Parser* p) -> lang::Node*
        {
          lang::Token tok;
          
          bool is = false;
          while (p->seek().which == MINUS)
          {
            p->eat(MINUS, tok);
            is = !is;
          }
          
          ExpressionNode* expr = p->parse<ExpressionNode>("unary_not_term");
          
          if (!is || !expr)
          {
            return expr;
          }
          else
          {
            ExpressionNode* node = new ExpressionNode(ExpressionNode::Unary::Neg, expr);
            node->setToken(tok);
            return node;
          }
        }
      );
      
      parser.grammars()["mul_term"] = lang::Grammar(
        [](lang::Parser* p) -> bool
        {
          return p->predicate("unary_neg_term");
        },
        
        [](lang::Parser* p) -> lang::Node*
        {
          lang::Token tok;
          
          ExpressionNode* expr = p->parse<ExpressionNode>("unary_neg_term");
          if (!expr)
          {
            return nullptr;
          }
          
          while (p->seek().which == TIMES ||
                 p->seek().which == SLASH)
          {
            ExpressionNode::Binary op;
            if (p->seek().which == TIMES)
            {
              p->eat(TIMES, tok);
              op = ExpressionNode::Binary::Mul;
            }
            else if (p->seek().which == SLASH)
            {
              p->eat(SLASH, tok);
              op = ExpressionNode::Binary::Div;
            }
            
            ExpressionNode* rhs = p->parse<ExpressionNode>("unary_neg_term");
            if (!rhs)
            {
              delete expr;
              return nullptr;
            }
            
            ExpressionNode* node = new ExpressionNode(op, expr, rhs);
            node->setToken(tok);
            expr = node;
          }
          
          return expr;
        }
      );
      
      parser.grammars()["add_term"] = lang::Grammar(
        [](lang::Parser* p) -> bool
        {
          return p->predicate("mul_term");
        },
        
        [](lang::Parser* p) -> lang::Node*
        {
          lang::Token tok;
          
          ExpressionNode* expr = p->parse<ExpressionNode>("mul_term");
          if (!expr)
          {
            return nullptr;
          }
          
          while (p->seek().which == PLUS ||
                 p->seek().which == MINUS)
          {
            ExpressionNode::Binary op;
            if (p->seek().which == MINUS)
            {
              p->eat(MINUS, tok);
              op = ExpressionNode::Binary::Sub;
            }
            else if (p->seek().which == PLUS)
            {
              p->eat(PLUS, tok);
              op = ExpressionNode::Binary::Add;
            }
            
            ExpressionNode* rhs = p->parse<ExpressionNode>("mul_term");
            if (!rhs)
            {
              delete expr;
              return nullptr;
            }
            
            ExpressionNode* node = new ExpressionNode(op, expr, rhs);
            node->setToken(tok);
            expr = node;
          }
          
          return expr;
        }
      );
      
      parser.grammars()["rel_term"] = lang::Grammar(
        [](lang::Parser* p) -> bool
        {
          return p->predicate("add_term");
        },
        
        [](lang::Parser* p) -> lang::Node*
        {
          lang::Token tok;
          
          ExpressionNode* expr = p->parse<ExpressionNode>("add_term");
          if (!expr)
          {
            return nullptr;
          }
          
          while (p->seek().which == LT ||
                 p->seek().which == LTE ||
                 p->seek().which == GT ||
                 p->seek().which == GTE ||
                 p->seek().which == EQ ||
                 p->seek().which == NEQ)
          {
            ExpressionNode::Binary op;
            if (p->seek().which == LT)
            {
              p->eat(LT, tok);
              op = ExpressionNode::Binary::Lt;
            }
            else if (p->seek().which == LTE)
            {
              p->eat(LTE, tok);
              op = ExpressionNode::Binary::Lte;
            }
            else if (p->seek().which == GT)
            {
              p->eat(GT, tok);
              op = ExpressionNode::Binary::Gt;
            }
            else if (p->seek().which == GTE)
            {
              p->eat(GTE, tok);
              op = ExpressionNode::Binary::Gte;
            }
            else if (p->seek().which == EQ)
            {
              p->eat(EQ, tok);
              op = ExpressionNode::Binary::Eq;
            }
            else if (p->seek().which == NEQ)
            {
              p->eat(NEQ, tok);
              op = ExpressionNode::Binary::Neq;
            }
            
            ExpressionNode* rhs = p->parse<ExpressionNode>("add_term");
            if (!rhs)
            {
              delete expr;
              return nullptr;
            }
            
            ExpressionNode* node = new ExpressionNode(op, expr, rhs);
            node->setToken(tok);
            expr = node;
          }
          
          return expr;
        }
      );
      
      parser.grammars()["expression"] = lang::Grammar(
        [](lang::Parser* p) -> bool
        {
          return p->predicate("rel_term");
        },
        
        [](lang::Parser* p) -> lang::Node*
        {
          lang::Token tok;
          
          ExpressionNode* expr = p->parse<ExpressionNode>("rel_term");
          if (!expr)
          {
            return nullptr;
          }
          
          while (p->seek().which == AND ||
                 p->seek().which == OR)
          {
            ExpressionNode::Binary op;
            if (p->seek().which == AND)
            {
              p->eat(AND, tok);
              op = ExpressionNode::Binary::And;
            }
            else if (p->seek().which == OR)
            {
              p->eat(OR, tok);
              op = ExpressionNode::Binary::Or;
            }
            
            ExpressionNode* rhs = p->parse<ExpressionNode>("rel_term");
            if (!rhs)
            {
              delete expr;
              return nullptr;
            }
            
            ExpressionNode* node = new ExpressionNode(op, expr, rhs);
            node->setToken(tok);
            expr = node;
          }
          
          return expr;
        }
      );
      
      parser.grammars()["variable_declaration"] = lang::Grammar(
        [](lang::Parser* p) -> bool
        {
          return p->seek().which == TYPENAME;
        },
        
        [](lang::Parser* p) -> lang::Node*
        {
          lang::Token tok;
          
          if (!p->eat(TYPENAME, tok))
          {
            p->error("expected type name");
            return nullptr;
          }
          
          core::Type type = tok.value.cast<core::Type>();
          
          if (!p->eat(IDENT, tok))
          {
            if (tok.which != VARIABLENAME)
            {
              p->error("expected identifier");
            }
            else
            {
              p->error("`" + tok.value.cast<std::string>() + "' is already declared");
            }
            return nullptr;
          }
          
          std::string name = tok.value.cast<std::string>();
          
          if (p->scope()->layer().findInScope(name))
          {
            p->error("variable `" + name + "' is already declared");
            return nullptr;
          }
          
          ExpressionNode* expr = nullptr;
          
          if (p->seek().which == EQUALS)
          {
            p->eat(EQUALS, tok);
            expr = p->parse<ExpressionNode>("expression");
            if (!expr)
            {
              return nullptr;
            }
          }
          
          lang::Token ptok = tok;
          
          core::Variable variable(name, type);
          p->scope()->layer().addElement(name, core::Symbol(variable));
          
          if (!p->eat(SEMICOLON, tok))
          {
            if (expr)
            {
              delete expr;
            }
            p->error("expected `;'");
            return nullptr;
          }
          
          VariableDeclNode* node = new VariableDeclNode(variable, expr);
          node->setToken(ptok);
          return node;
        }
      );
      
      parser.grammars()["variable_assign"] = lang::Grammar(
        [](lang::Parser* p) -> bool
        {
          return p->seek().which == VARIABLENAME;
        },
        
        [](lang::Parser* p) -> lang::Node*
        {
          lang::Token tok;
          if (!p->eat(VARIABLENAME, tok))
          {
            p->error("expected variable identifier");
            return nullptr;
          }
          core::Variable variable = tok.value.cast<core::Variable>();
          
          if (!p->eat(EQUALS, tok))
          {
            p->error("expected `='");
            return nullptr;
          }
          
          lang::Token ptok = tok;
          
          ExpressionNode* expr = p->parse<ExpressionNode>("expression");
          if (!expr)
          {
            return nullptr;
          }
          
          if (!p->eat(SEMICOLON, tok))
          {
            p->error("expected `;'");
            delete expr;
            return nullptr;
          }
          
          VariableAssignNode* node = new VariableAssignNode(variable, expr);
          node->setToken(ptok);
          return node;
        }
      );
      
      parser.grammars()["statement"] = lang::Grammar(
        [](lang::Parser* p) -> bool
        {
          return p->predicate("variable_declaration") ||
                 p->predicate("variable_assign");
        },
        
        [](lang::Parser* p) -> lang::Node*
        {
          if (p->predicate("variable_assign"))
          {
            return p->parse("variable_assign");
          }
          else // if (p->predicate("variable_declaration"))
          {
            return p->parse("variable_declaration");
          }
        }
      );
      
      parser.grammars()["statement_block"] = lang::Grammar(
        [](lang::Parser* p) -> bool
        {
          return p->predicate("statement");
        },
        
        [](lang::Parser* p) -> lang::Node*
        {
          StatementBlockNode* block = new StatementBlockNode();
          
          do
          {
            lang::Node* statement = p->parse("statement");
            if (!statement)
            {
              delete block;
              return nullptr;
            }
            
            block->children().push_back(statement);
          } while (p->predicate("statement"));
          
          return block;
        }
      );
      
      parser.grammars()["function_declaration"] = lang::Grammar(
       [](lang::Parser* p) -> bool
       {
         return p->seek().which == TYPENAME;
       },
     
       [](lang::Parser* p) -> lang::Node*
       {
         lang::Token tok;
         
         p->eat(TYPENAME, tok);
         core::Type ret = tok.value.cast<core::Type>();
         
         if (!p->eat(IDENT, tok))
         {
           p->error("expected identifier");
           return nullptr;
         }
         lang::Token ptok = tok;
         std::string name = tok.value.cast<std::string>();
         
         core::Prototype prototype(ret);
         
         if (!p->eat(LPAR, tok))
         {
           p->error("expected `('");
           return nullptr;
         }
         
         p->scope()->down();
         
         if (p->seek().which != RPAR)
         {
           do
           {
             if (p->seek().which == COMMA)
             {
               p->eat(COMMA, tok);
             }
             
             if (!p->eat(TYPENAME, tok))
             {
               p->error("expected typename");
               return nullptr;
             }
             core::Type type = tok.value.cast<core::Type>();
             
             std::string name;
             if (p->seek().which == IDENT)
             {
               p->eat(IDENT, tok);
               name = tok.value.cast<std::string>();
             }
             else if (p->seek().which == VARIABLENAME)
             {
               p->eat(VARIABLENAME, tok);
               name = tok.value.cast<core::Variable>().name();
              
               if (p->scope()->layer().findInScope(name))
               {
                 p->error("redeclaration of variable `" + name + "'");
                 return nullptr;
               }
               
             }
             else if (p->seek().which == FUNCTIONNAME)
             {
               p->eat(FUNCTIONNAME, tok);
               name = tok.value.cast<core::Function>().name();
             }
             else
             {
               p->error("ecpected identifier");
               return nullptr;
             }
             
             core::Variable variable(name, type);
             p->scope()->layer().addElement(name, variable);
             prototype.args().push_back(variable);
           } while (p->seek().which == COMMA);
         }
         
         if (!p->eat(RPAR, tok))
         {
           p->error("expected `)'");
           return nullptr;
         }
         
         core::Function function(name, prototype);
         p->scope()->layer().parent()->addElement(name, function);
         FunctionDeclarationNode* node = new FunctionDeclarationNode(function);
         node->setToken(ptok);
         
         if (!p->eat(LCURLY, tok))
         {
           delete node;
           p->error("expected `{'");
           return nullptr;
         }
         
         if (p->seek().which != RCURLY)
         {
           lang::Node* block = p->parse("statement_block");
           if (!block)
           {
             delete node;
             return nullptr;
           }
           
           node->addChild(block);
         }
         
         if (!p->eat(RCURLY, tok))
         {
           p->error("expected `}'");
           delete node;
           return nullptr;
         }
         
         p->scope()->up();
         
         return node;
       }
     );

    }
  }
}
