#ifndef __GPSE_LANG_PARSER_H__
#define __GPSE_LANG_PARSER_H__

#include "lang/lexer.hpp"
#include "lang/node.hpp"
#include "lang/grammar.hpp"

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

            bool good() const;
            void reset();
            bool eat(int which, Token& token);
            Token seek();
            bool predicate(std::string const& grammar);
            Node* parseRaw(std::string const& grammar);
            void error(std::string const& message, Token const& tok) const;
            void error(std::string const& message) const;

            template <typename T = lang::Node>
            T* parse(std::string const& grammar)
            {
                return reinterpret_cast<T*>(parseRaw(grammar));
            }

        private:
            Token m_advance();

        private:
            Lexer& m_lexer;
            Token m_current;
            Token m_next;
            bool m_good;
            std::map<std::string, Grammar> m_grammars;
        };
    }
}

#endif // __GPSE_LEX_PARSER_H__
