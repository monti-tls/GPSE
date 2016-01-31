#ifndef __GPSE_SKETCH_RUNTIME_DEF_H__
#define __GPSE_SKETCH_RUNTIME_DEF_H__

#include "lang/lexer.hpp"
#include "lang/parser.hpp"

#include <iostream>

namespace gpse
{
    namespace sketch
    {
        void setupScope(core::Scope* scope);
    }
}

#endif // __GPSE_SKETCH_RUNTIME_DEF_H__
