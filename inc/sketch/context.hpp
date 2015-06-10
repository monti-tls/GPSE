#ifndef __GPSE_SKETCH_CONTEXT_H__
#define __GPSE_SKETCH_CONTEXT_H__

#include "core/namedscope.hpp"
#include "core/variablename.hpp"
#include "core/type.hpp"

namespace gpse
{
  namespace sketch
  {
    class Context
    {
    public:
      Context();
      ~Context();
      
      core::NamedScope<std::string, core::Type>& typeScope();
      core::NamedScope<std::string, core::Type>& typeScopeDown();
      core::NamedScope<std::string, core::Type>& typeScopeUp();
      
      core::NamedScope<core::VariableName, core::Type>& variableScope();
      core::NamedScope<core::VariableName, core::Type>& variableScopeDown();
      core::NamedScope<core::VariableName, core::Type>& variableScopeUp();
      
    private:
      core::NamedScope<std::string, core::Type>* _m_typeScope;
      core::NamedScope<core::VariableName, core::Type>* _m_variableScope;
    };
  }
}

#endif // __GPSE_SKETCH_CONTEXT_H__
