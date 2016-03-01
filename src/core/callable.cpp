#include "core/callable.hpp"

using namespace core;

Callable::~Callable()
{}

Object Callable::invoke(std::vector<Object> const& args) const
{
    if (m_kind == Kind::Native)
        return M_nativeInvocation(args);
    else // if (m_kind == Scripted)
        return M_scriptedInvocation(args);
}

Object Callable::M_nativeInvocation(std::vector<Object> const& args) const
{ return m_meta.as<NativeMetaType>()->invoke(args); }

Object Callable::M_scriptedInvocation(std::vector<Object> const& args) const
{ return Object::Nil; }
