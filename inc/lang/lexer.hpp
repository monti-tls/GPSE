#ifndef __GPSE_LANG_LEXER_H__
#define __GPSE_LANG_LEXER_H__

#include "core/some.hpp"
#include "lang/token.hpp"
#include "lang/rule.hpp"
#include "core/scope.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <functional>

namespace gpse
{
  namespace lang
  {
    class Lexer
    {
    public:      
      typedef std::function<bool (int)> wsrule_t;
      
    public:
      Lexer(std::istream& in, core::Scope* scope = nullptr);
      ~Lexer();
      
      wsrule_t& wsRule();
      wsrule_t const& wsRule() const;
      std::vector<Rule>& rules();
      std::vector<Rule> const& rules() const;
      core::Scope* scope() const;
      
      bool good() const;
      void reset();
      int hint();
      int get();
      bool eat(char c);
      bool eat(std::string const& str);
      void skipWs();
      void error(Token const& tok, std::string const& message);
      
      Token token();
      
    private:
      std::string _M_wholeLine(Token const& tok);
      
    private:
      std::istream& _m_in;
      core::Scope* _m_scope;
      int _m_line;
      int _m_col;
      int _m_pos;
      bool _m_good;
      wsrule_t _m_wsRule;
      std::vector<Rule> _m_rules;
    };
  }
}

#endif // __GPSE_LEX_LEXER_H__
