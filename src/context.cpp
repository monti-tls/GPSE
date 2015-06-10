#include "sketch/context.hpp"

using namespace gpse;
using namespace sketch;

Context::Context() :
  _m_typeScope(new core::NamedScope<std::string, core::Type>()),
  _m_variableScope(new core::NamedScope<core::VariableName, core::Type>())
{
  _m_typeScope->addElement("int", core::Type::Integer);
  _m_typeScope->addElement("float", core::Type::Floating);
  _m_typeScope->addElement("string", core::Type::String);
  _m_typeScope->addElement("bool", core::Type::Boolean);
}

Context::~Context()
{
  for (; _m_typeScope->parent(); _m_typeScope = _m_typeScope->parent());
  delete _m_typeScope;
  
  for (; _m_variableScope->parent(); _m_variableScope = _m_variableScope->parent());
  delete _m_variableScope;
}

core::NamedScope<std::string, core::Type>& Context::typeScope()
{
  return *_m_typeScope;
}

core::NamedScope<std::string, core::Type>& Context::typeScopeDown()
{
  if (!_m_typeScope->child())
  {
    core::NamedScope<std::string, core::Type>* child = new core::NamedScope<std::string, core::Type>(_m_typeScope);
    _m_typeScope->setChild(child);
  }
  
  _m_typeScope = _m_typeScope->child();
  
  return *_m_typeScope;
}

core::NamedScope<std::string, core::Type>& Context::typeScopeUp()
{
  if (_m_typeScope->parent())
  {
    _m_typeScope = _m_typeScope->parent();
  }
  
  return *_m_typeScope;
}

core::NamedScope<core::VariableName, core::Type>& Context::variableScope()
{
  return *_m_variableScope;
}

core::NamedScope<core::VariableName, core::Type>& Context::variableScopeDown()
{
  if (!_m_variableScope->child())
  {
    core::NamedScope<core::VariableName, core::Type>* child = new core::NamedScope<core::VariableName, core::Type>(_m_variableScope);
    _m_variableScope->setChild(child);
  }
  
  _m_variableScope = _m_variableScope->child();
  
  return *_m_variableScope;
}

core::NamedScope<core::VariableName, core::Type>& Context::variableScopeUp()
{
  if (_m_variableScope->parent())
  {
    _m_variableScope = _m_variableScope->parent();
  }
  
  return *_m_variableScope;
}
