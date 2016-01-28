#include "core/function.hpp"

using namespace gpse;
using namespace core;

Prototype::Prototype() :
  _m_ret(Type::Nil),
  _m_args()
{
  
}

Prototype::Prototype(Type const& ret, std::vector<Variable> const& args) :
  _m_ret(ret),
  _m_args(args)
{
  
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

Function::Function() :
  _m_name(),
  _m_prototype(),
  _m_node(nullptr)
{
  
}

Function::Function(std::string const& name, Prototype const& prototype) :
  _m_name(name),
  _m_prototype(prototype)
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
