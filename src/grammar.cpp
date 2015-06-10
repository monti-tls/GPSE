#include "lang/grammar.hpp"

using namespace gpse;
using namespace lang;

Grammar::Grammar(Grammar::pred_t predicate, Grammar::get_t get) :
  _m_predicate(predicate),
  _m_get(get)
{
  
}

Grammar::~Grammar()
{

}

bool Grammar::predicate(Parser* parser)
{
  return _m_predicate(parser);
}

Node* Grammar::get(Parser* parser)
{
  return _m_get(parser);
}
