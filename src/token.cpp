#include "lang/token.hpp"

namespace gpse {
  namespace lang
  {
    Token const eofToken = Token(Token::Eof);
    Token const badToken = Token(Token::Bad);
  }
}

using namespace gpse;
using namespace lang;

Token::Token(int which, core::Some const& value) :
  which(which),
  value(value)
{
}

bool Token::operator==(Token const& rhs)
{
  return which == rhs.which;
}

bool Token::operator!=(Token const& rhs)
{
  return !operator==(rhs);
}
