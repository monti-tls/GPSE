#ifndef __GPSE_CORE_SYMBOL_H__
#define __GPSE_CORE_SYMBOL_H__

#include "core/some.hpp"
#include "core/variable.hpp"
#include "core/type.hpp"

namespace gpse
{
  namespace core
  {
    class Symbol
    {
    public:
      Symbol();
      Symbol(Variable const& variable);
      Symbol(Type const& type);
      ~Symbol();
      
      bool isVariable() const;
      bool isType() const;
      
      Variable const& variable() const;
      Type const& type() const;
      
    private:
      Some _m_value;
    };
  }
}

#endif // __GPSE_CORE_SYMBOL_H__
