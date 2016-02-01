#include "core/callback.hpp"

using namespace gpse;
using namespace core;
using namespace detail;

core::Some Callback::call(std::vector<core::Some> const& args) const
{
    return _m_impl->call(args);
}

core::Prototype Callback::prototype() const
{
    return _m_impl->prototype();
}

std::string const& Callback::name() const
{
    return _m_name;
}