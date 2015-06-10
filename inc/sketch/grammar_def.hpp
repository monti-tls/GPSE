#ifndef __GPSE_SKETCH_GRAMMAR_DEF_H__
#define __GPSE_SKETCH_GRAMMAR_DEF_H__

#include "lang/lexer.hpp"
#include "lang/parser.hpp"

#include <iostream>

namespace gpse
{
  namespace sketch
  {
    void setupParser(lang::Parser& parser);
  }
}

#endif // __GPSE_SKETCH_GRAMMAR_DEF_H__
