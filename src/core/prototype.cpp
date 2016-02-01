#include "core/prototype.hpp"

using namespace gpse;
using namespace core;

Prototype::Prototype()
    : _m_ret(Type::Nil)
    , _m_args()
{
}

Prototype::Prototype(Type const& ret, std::vector<Variable> const& args)
    : _m_ret(ret)
    , _m_args(args)
{
}

Prototype::Prototype(Type const& ret, std::vector<Type> const& args_types)
    : _m_ret(ret)
{
    for(auto& tp : args_types)
    {
        _m_args.push_back(core::Variable("_", tp));
    }
}

Prototype::~Prototype()
{
}

Type const& Prototype::ret() const
{
    return _m_ret;
}

void Prototype::setRet(Type const& ret)
{
    _m_ret = ret;
}

std::vector<Variable>& Prototype::args()
{
    return _m_args;
}

std::vector<Variable> const& Prototype::args() const
{
    return _m_args;
}
