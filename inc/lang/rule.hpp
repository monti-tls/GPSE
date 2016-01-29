#ifndef __GPSE_LANG_RULE_H__
#define __GPSE_LANG_RULE_H__

#include "lang/token.hpp"

#include <functional>

namespace gpse
{
    namespace lang
    {
        class Lexer;

        class Rule
        {
        public:
            typedef std::function<bool(int)> pred_t;
            typedef std::function<Token(Lexer*)> get_t;

        public:
            Rule(pred_t predicate, get_t get);
            ~Rule();

            bool predicate(int hint);
            Token get(Lexer* lexer);

            static Rule single(int which, char c);
            static Rule single(int which, std::string const& str);

        private:
            pred_t _m_predicate;
            get_t _m_get;
        };
    }
}

#endif // __GPSE_LEX_RULE_H__
