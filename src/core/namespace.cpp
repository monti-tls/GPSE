#include "core/namespace.hpp"

using namespace core;

Namespace::Namespace()
{ m_current = &m_root; }

Namespace::~Namespace()
{}

void Namespace::set(std::string const& name)
{ m_current->set(name); }

bool Namespace::find(std::string const& name, Symbol** found)
{ return m_current->find(name, found); }

bool Namespace::findLocal(std::string const& name, Symbol** found)
{ return m_current->findLocal(name, found); }

void Namespace::down()
{ m_current = m_current->addChild(); }

void Namespace::up()
{ m_current = m_current->parent(); }
