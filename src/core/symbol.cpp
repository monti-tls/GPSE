#include "core/symbol.hpp"

using namespace gpse;
using namespace core;

Symbol::Symbol()
    : _m_value()
{
}

Symbol::Symbol(Variable const& Variable)
    : _m_value(Variable)
{
}

Symbol::Symbol(Type const& type)
    : _m_value(type)
{
}

Symbol::Symbol(Function const& function)
    : _m_value(function)
{
}

Symbol::Symbol(Callback const& callback)
    : _m_value(callback)
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

bool Symbol::isCallback() const
{
    return _m_value.is<Callback>();
}

Variable& Symbol::variable()
{
    return _m_value.as<Variable>();
}

Variable const& Symbol::variable() const
{
    return _m_value.as<Variable>();
}

Type& Symbol::type()
{
    return _m_value.as<Type>();
}

Type const& Symbol::type() const
{
    return _m_value.as<Type>();
}

Function& Symbol::function()
{
    return _m_value.as<Function>();
}

Function const& Symbol::function() const
{
    return _m_value.as<Function>();
}

Callback& Symbol::callback()
{
    return _m_value.as<Callback>();
}

Callback const& Symbol::callback() const
{
    return _m_value.as<Callback>();
}
