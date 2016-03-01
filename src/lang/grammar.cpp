#include "lang/grammar.hpp"

using namespace gpse;
using namespace lang;

Grammar::Grammar(Grammar::pred_t predicate, Grammar::get_t get)
    : m_predicate(predicate)
    , m_get(get)
{
}

Grammar::~Grammar()
{
}

bool Grammar::predicate(Parser* parser)
{
    return m_predicate(parser);
}

Node* Grammar::get(Parser* parser)
{
    return m_get(parser);
}
