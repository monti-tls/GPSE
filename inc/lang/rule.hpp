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
            static Rule doubled(int which_first, int which_second, char first, char second);

        private:
            pred_t m_predicate;
            get_t m_get;
        };
    }
}

#endif // __GPSE_LEX_RULE_H__
