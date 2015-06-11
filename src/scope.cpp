#include "core/scope.hpp"

using namespace gpse;
using namespace core;

ScopeLayer::ScopeLayer(ScopeLayer* parent) :
  _m_parent(parent)
{}

ScopeLayer::~ScopeLayer()
{
  for (auto child : _m_children)
  {
    delete child;
  }
}

void ScopeLayer::setParent(ScopeLayer* parent)
{
  _m_parent = parent;
}

ScopeLayer* ScopeLayer::parent() const
{
  return _m_parent;
}

std::vector<ScopeLayer*>& ScopeLayer::children()
{
  return _m_children;
}

std::vector<ScopeLayer*> const& ScopeLayer::children() const
{
  return _m_children;
}

void ScopeLayer::addElement(std::string const& name, Symbol const& element)
{
  _m_content[name] = element;
}

bool ScopeLayer::find(std::string const& name, Symbol* element)
{
  auto it = _m_content.find(name);
  if (it == _m_content.end())
  {
    if (_m_parent)
    {
      return _m_parent->find(name, element);
    }
    
    return false;
  }
  
  if (element)
  {
    *element = it->second;
  }
  
  return true;
}

bool ScopeLayer::findInScope(std::string const& name, Symbol* element)
{
  auto it = _m_content.find(name);
  if (it != _m_content.end())
  {
    if (element)
    {
      *element = it->second;
    }
    return true;
  }
  
  return false;
}

Scope::Scope() :
  _m_currentLayer(new ScopeLayer())
{
  
}

Scope::~Scope()
{
  for (; _m_currentLayer->parent(); _m_currentLayer = _m_currentLayer->parent());
  delete _m_currentLayer;
}

ScopeLayer& Scope::layer()
{
  return *_m_currentLayer;
}

ScopeLayer const& Scope::layer() const
{
  return *_m_currentLayer;
}

ScopeLayer& Scope::down()
{
  ScopeLayer* child = new ScopeLayer(_m_currentLayer);
  _m_currentLayer->children().push_back(child);
  _m_currentLayer = child;
  return *_m_currentLayer;
}

ScopeLayer& Scope::up()
{
  if (_m_currentLayer->parent())
  {
    _m_currentLayer = _m_currentLayer->parent();
  }
  
  return *_m_currentLayer;
}
