#ifndef __GPSE_LANG_PARSER_H__
#define __GPSE_LANG_PARSER_H__

#include "lang/lexer.hpp"
#include "lang/node.hpp"
#include "lang/grammar.hpp"
#include "core/scope.hpp"

#include <map>

namespace gpse
{
  namespace lang
  {
    class Parser
    {
    public:
      Parser(Lexer& lex);
      ~Parser();
      
      std::map<std::string, Grammar>& grammars();
      std::map<std::string, Grammar> const& grammars() const;
      core::Scope* scope() const;
      
      bool good() const;
      void reset();
      bool eat(int which, Token& token);
      Token seek();
      bool predicate(std::string const& grammar);
      Node* parseRaw(std::string const& grammar);
      void error(std::string const& message);
      
      template <typename T = lang::Node>
      T* parse(std::string const& grammar)
      { return reinterpret_cast<T*>(parseRaw(grammar)); }
      
    private:
      Token _M_advance();
      
    private:
      Lexer& _m_lexer;
      Token _m_current;
      Token _m_next;
      bool _m_good;
      std::map<std::string, Grammar> _m_grammars;
    };
  }
}

#endif // __GPSE_LEX_PARSER_H__
