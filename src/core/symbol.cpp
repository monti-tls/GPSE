#include "core/symbol.hpp"

using namespace core;

Symbol::Symbol(std::string const& name)
	: m_name(name)
	, m_target(nullptr)
{}

std::string const& Symbol::name() const
{ return m_name; }

void Symbol::bind(Object* target)
{ m_target = target; }

bool Symbol::bound() const
{ return m_target; }

Object* Symbol::target() const
{ return m_target; }
