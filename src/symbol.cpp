#include "core/symbol.hpp"

using namespace gpse;
using namespace core;

Symbol::Symbol() :
  _m_value()
{
  
}

Symbol::Symbol(VariableName const& variableName) :
  _m_value(variableName)
{
  
}

Symbol::Symbol(Type const& type) :
  _m_value(type)
{
  
}

Symbol::~Symbol()
{
  
}

bool Symbol::isVariableName() const
{
  return _m_value.is<VariableName>();
}

bool Symbol::isType() const
{
  return _m_value.is<Type>();
}

VariableName const& Symbol::variableName() const
{
  return _m_value.cast<VariableName>();
}

Type const& Symbol::type() const
{
  return _m_value.cast<Type>();
}
