#include "sketch/rule_def.hpp"
#include "sketch/context.hpp"
#include "core/type.hpp"

namespace gpse
{
  namespace sketch
  {
    void setupLexer(lang::Lexer& lexer)
    {
      lexer.rules().clear();
      
      lexer.wsRule() = [](int h) -> bool
      {
        return std::isspace(h) || h == '\n';
      };
      
      lexer.rules().push_back(lang::Rule::single(LPAR, '('));
      lexer.rules().push_back(lang::Rule::single(RPAR, ')'));
      lexer.rules().push_back(lang::Rule::single(TIMES, '*'));
      lexer.rules().push_back(lang::Rule::single(SLASH, '/'));
      lexer.rules().push_back(lang::Rule::single(PLUS, '+'));
      lexer.rules().push_back(lang::Rule::single(MINUS, '-'));
      lexer.rules().push_back(lang::Rule::single(AND, "&&"));
      lexer.rules().push_back(lang::Rule::single(OR, "||"));
      lexer.rules().push_back(lang::Rule::single(NOT, '!'));
      lexer.rules().push_back(lang::Rule::single(SEMICOLON, ';'));
      lexer.rules().push_back(lang::Rule::single(QUOTE, '"'));
      
      // LT / LTE
      lexer.rules().push_back(lang::Rule(
        [](int h) -> bool
        {
          return h == '<';
        },
        
        [](lang::Lexer* l) -> lang::Token
        {
          l->eat('<');
          
          if (l->hint() == '=')
          {
            l->eat('=');
            return lang::Token(LTE);
          }
          
          return lang::Token(LT);
        }
      ));
      
      // GT / GTE
      lexer.rules().push_back(lang::Rule(
        [](int h) -> bool
        {
          return h == '>';
        },
        
        [](lang::Lexer* l) -> lang::Token
        {
          l->eat('>');
          
          if (l->hint() == '=')
          {
            l->eat('=');
            return lang::Token(GTE);
          }
          
          return lang::Token(GT);
        }
      ));
      
      // EQ / EQUALS
      lexer.rules().push_back(lang::Rule(
        [](int h) -> bool
        {
          return h == '=';
        },
        
        [](lang::Lexer* l) -> lang::Token
        {
          l->eat('=');
          
          if (l->hint() == '=')
          {
            l->eat('=');
            return lang::Token(EQ);
          }
          
          return lang::Token(EQUALS);
        }
      ));
      
      lexer.rules().push_back(lang::Rule::single(NEQ, "!="));
      
      // NUMBER
      lexer.rules().push_back(lang::Rule(
        [](int h) -> bool
        {
          return std::isdigit(h);
        },
        
        [](lang::Lexer* l) -> lang::Token
        {
          std::string temp;
          bool integer = true;
          
          while (std::isdigit(l->hint()))
          {
            temp += l->get();
          }
          
          if (l->hint() == '.')
          {
            integer = false;
            
            temp += l->get();
            
            while (std::isdigit(l->hint()))
            {
              temp += l->get();
            }
          }
          
          if (integer)
          {
            return lang::Token(NUMBER, static_cast<int>(std::atoi(temp.c_str())));
          }
          else
          {
            return lang::Token(NUMBER, static_cast<float>(std::atof(temp.c_str())));
          }
        }
      ));
      
      // IDENT, VARIABLENAME, TYPENAME and K_*
      lexer.rules().push_back(lang::Rule(
        [](int h) -> bool
        {
          return std::isalpha(h) || h == '_';
        },
        
        [](lang::Lexer* l) -> lang::Token
        {
          std::string temp;
          
          while (std::isalpha(l->hint()) || l->hint() == '_')
          {
            temp += l->get();
          }
          
          while (std::isalnum(l->hint()) || l->hint() == '_')
          {
            temp += l->get();
          }
          
          core::Type type;
          if (temp == "true")
          {
            return lang::Token(K_TRUE);
          }
          else if (temp == "false")
          {
            return lang::Token(K_FALSE);
          }
          if (l->context().cast<Context*>()->typeScope().find(temp, &type))
          {
            return lang::Token(TYPENAME, type);
          }
          else if (l->context().cast<Context*>()->variableScope().find(temp))
          {
            return lang::Token(VARIABLENAME, temp);
          }
          
          return lang::Token(IDENT, temp);
        }
      ));
      
      // STRING
      lexer.rules().push_back(lang::Rule(
        [](int h) -> bool
        {
          return h == '"';
        },
        
        [](lang::Lexer* l) -> lang::Token
        {
          std::string temp;
          
          l->eat('"');
          
          for (int r = l->get(); r != '"' && r != -1; r = l->get())
          {
            if (r == '\\')
            {
              r = l->get();
              if (r == -1)
                break;
              if (r == 'n')
                temp += '\n';
              else if (r == 'r')
                temp += '\r';
              else if (r == 't')
                temp += '\t';
              else
                return lang::badToken;
            }
            else
            {
              temp += r;
            }
          }
          
          l->eat('"');
          
          return lang::Token(IDENT, temp);
        }
      ));
    }
  }
}
