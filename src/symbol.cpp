#include "core/symbol.hpp"

using namespace gpse;
using namespace core;

Symbol::Symbol() :
  _m_value()
{
  
}

Symbol::Symbol(Variable const& Variable) :
  _m_value(Variable)
{
  
}

Symbol::Symbol(Type const& type) :
  _m_value(type)
{
  
}

Symbol::Symbol(Function const& function) :
  _m_value(function)
{
  
}

Symbol::~Symbol()
{
  
}

bool Symbol::isVariable() const
{
  return _m_value.is<Variable>();
}

bool Symbol::isType() const
{
  return _m_value.is<Type>();
}

bool Symbol::isFunction() const
{
  return _m_value.is<Function>();
}

Variable& Symbol::variable()
{
  return _m_value.cast<Variable>();
}

Variable const& Symbol::variable() const
{
  return _m_value.cast<Variable>();
}

Type& Symbol::type()
{
  return _m_value.cast<Type>();
}

Type const& Symbol::type() const
{
  return _m_value.cast<Type>();
}

Function& Symbol::function()
{
  return _m_value.cast<Function>();
}

Function const& Symbol::function() const
{
  return _m_value.cast<Function>();
}
