#include "core/function.hpp"

using namespace gpse;
using namespace core;

Function::Function()
    : _m_name()
    , _m_prototype()
    , _m_node(nullptr)
{
}

Function::Function(std::string const& name, Prototype const& prototype)
    : _m_name(name)
    , _m_prototype(prototype)
{
}

Function::~Function()
{
}

std::string const& Function::name() const
{
    return _m_name;
}

Prototype const& Function::prototype() const
{
    return _m_prototype;
}

void Function::setNode(lang::Node* node)
{
    _m_node = node;
}

lang::Node* Function::node() const
{
    return _m_node;
}
