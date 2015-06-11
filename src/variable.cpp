#include "core/variable.hpp"

using namespace gpse;
using namespace core;

Variable::Variable() :
  _m_name(),
  _m_type()
{
  
}

Variable::Variable(std::string const& name, core::Type const& type) :
  _m_name(name),
  _m_type(type)
{
  
}

Variable::~Variable()
{
  
}

void Variable::setName(std::string const& name)
{
  _m_name = name;
}

std::string const& Variable::name() const
{
  return _m_name;
}

void Variable::setType(core::Type const& type)
{
  _m_type = type;
}

core::Type const& Variable::type() const
{
  return _m_type;
}
