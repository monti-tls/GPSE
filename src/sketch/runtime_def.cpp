#include "sketch/runtime_def.hpp"
#include "core/scope.hpp"
#include "core/type.hpp"

namespace gpse
{
    namespace sketch
    {
        void setupScope(core::Scope* scope)
        {
            scope->layer().addElement("nil", core::Symbol(core::Type::Nil));
            scope->layer().addElement("int", core::Symbol(core::Type::Integer));
            scope->layer().addElement("float", core::Symbol(core::Type::Floating));
            scope->layer().addElement("string", core::Symbol(core::Type::String));
            scope->layer().addElement("bool", core::Symbol(core::Type::Boolean));
            scope->layer().addElement("opaque", core::Symbol(core::Type::Opaque));
        }
    }
}
