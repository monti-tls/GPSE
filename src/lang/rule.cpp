#include "lang/rule.hpp"
#include "lang/lexer.hpp"

using namespace gpse;
using namespace lang;

Rule::Rule(Rule::pred_t predicate, Rule::get_t get)
    : _m_predicate(predicate)
    , _m_get(get)
{
}

Rule::~Rule()
{
}

bool Rule::predicate(int hint)
{
    return _m_predicate(hint);
}

Token Rule::get(Lexer* lexer)
{
    return _m_get(lexer);
}

Rule Rule::single(int which, char c)
{
    return Rule([c](int h)
                {
                    return h == c;
                },

                [which, c](Lexer* l)
                {
                    l->eat(c);
                    return Token(which);
                });
}

Rule Rule::single(int which, std::string const& str)
{
    return Rule([str](int h)
                {
                    return h == str.front();
                },

                [which, str](Lexer* l)
                {
                    l->eat(str);
                    return Token(which);
                });
}
