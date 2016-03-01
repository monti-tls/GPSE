#ifndef __GPSE_LANG_LEXER_H__
#define __GPSE_LANG_LEXER_H__

#include "lang/token.hpp"
#include "lang/rule.hpp"
#include "core/namespace.hpp"

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
            typedef std::function<bool(int)> wsrule_t;

        public:
            Lexer(std::istream& in, core::Namespace& ns);
            ~Lexer();

            core::Namespace& ns() const;
            wsrule_t& wsRule();
            wsrule_t const& wsRule() const;
            std::vector<Rule>& rules();
            std::vector<Rule> const& rules() const;

            bool good() const;
            void reset();
            int hint();
            int get();
            bool eat(char c);
            bool eat(std::string const& str);
            void skipWs();
            void error(Token const& tok, std::string const& message) const;

            Token token();

        private:
            std::string M_wholeLine(Token const& tok) const;

        private:
            std::istream& m_in;
            core::Namespace& m_ns;
            int m_line;
            int m_col;
            int m_pos;
            bool m_good;
            wsrule_t m_wsRule;
            std::vector<Rule> m_rules;
        };
    }
}

#endif // __GPSE_LEX_LEXER_H__
