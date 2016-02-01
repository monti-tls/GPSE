#ifndef __GPSE_SKETCH_RUNTIME_DEF_H__
#define __GPSE_SKETCH_RUNTIME_DEF_H__

#include "lang/lexer.hpp"
#include "lang/parser.hpp"
#include "core/callback.hpp"

#include <iostream>
#include <string>
#include <functional>

namespace gpse
{
    namespace sketch
    {
        void setupScope(core::Scope* scope);

        template <typename R, typename... A>
        void expose(core::Scope* scope, std::string const& name, R (*fun)(A...))
        {
            scope->layer().addElement(name, core::Callback(std::function<R(A...)>(fun), name));
        }
    }
}

#endif // __GPSE_SKETCH_RUNTIME_DEF_H__
