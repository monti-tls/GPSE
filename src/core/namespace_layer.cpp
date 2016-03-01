#include "core/namespace_layer.hpp"

using namespace core;

NamespaceLayer::NamespaceLayer(NamespaceLayer* parent)
	: m_parent(parent)
{}

NamespaceLayer::~NamespaceLayer()
{}

void NamespaceLayer::set(std::string const& name)
{ m_symbols[name] = Symbol(name); }

bool NamespaceLayer::find(std::string const& name, Symbol** found)
{
	std::map<std::string, Symbol>::iterator it;
	it = m_symbols.find(name);

	if (it != m_symbols.end())
	{
		if (found)
			*found = &it->second;

		return true;
	}

	for (auto child : m_children)
			if (child.find(name, found))
				return true;

	return false;
}

bool NamespaceLayer::findLocal(std::string const& name, Symbol** found)
{
	std::map<std::string, Symbol>::iterator it;
	it = m_symbols.find(name);

	if (it != m_symbols.end())
	{
		if (found)
			*found = &it->second;

		return true;
	}

	return false;
}

NamespaceLayer* NamespaceLayer::addChild()
{
	m_children.push_back(NamespaceLayer(this));

	return &m_children.back();
}

NamespaceLayer* NamespaceLayer::parent() const
{ return m_parent; }