#ifndef __GPSE_SKETCH_RULE_DEF_H__
#define __GPSE_SKETCH_RULE_DEF_H__

#include "lang/lexer.hpp"
#include "lang/parser.hpp"

#include <iostream>

namespace gpse
{
  namespace sketch
  {
    enum
    {
      LPAR = lang::Token::Good,
      RPAR,
      TIMES,
      SLASH,
      PLUS,
      MINUS,
      AND,
      OR,
      NOT,
      LT,
      LTE,
      GT,
      GTE,
      EQ,
      EQUALS,
      SEMICOLON,
      QUOTE,
      NEQ,
      NUMBER,
      STRING,
      IDENT,
      TYPENAME,
      VARIABLENAME,
      K_TRUE,
      K_FALSE
    };
    
    void setupLexer(lang::Lexer& lexer);
  }
}

#endif // __GPSE_SKETCH_RULE_DEF_H__