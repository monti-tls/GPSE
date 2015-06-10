#ifndef __GPSE_CORE_SYMBOL_H__
#define __GPSE_CORE_SYMBOL_H__

#include "core/some.hpp"
#include "core/variablename.hpp"
#include "core/type.hpp"

namespace gpse
{
  namespace core
  {
    class Symbol
    {
    public:
      Symbol();
      Symbol(VariableName const& variable);
      Symbol(Type const& type);
      ~Symbol();
      
      bool isVariableName() const;
      bool isType() const;
      
      VariableName const& variableName() const;
      Type const& type() const;
      
    private:
      Some _m_value;
    };
  }
}

#endif // __GPSE_CORE_SYMBOL_H__
