#include "core/scope.hpp"

using namespace gpse;
using namespace core;

ScopeLayer::ScopeLayer(ScopeLayer* parent, ScopeLayer* child) :
_m_parent(parent),
_m_child(child)
{}

ScopeLayer::~ScopeLayer()
{
  delete _m_child;
}

void ScopeLayer::setParent(ScopeLayer* parent)
{
  _m_parent = parent;
}

ScopeLayer* ScopeLayer::parent() const
{
  return _m_parent;
}

void ScopeLayer::setChild(ScopeLayer* child)
{
  _m_child = child;
}

ScopeLayer* ScopeLayer::child() const
{
  return _m_child;
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
  if (!_m_currentLayer->child())
  {
    _m_currentLayer->setChild(new ScopeLayer(_m_currentLayer));
  }
  
  _m_currentLayer = _m_currentLayer->child();
  return *_m_currentLayer;
}

ScopeLayer& Scope::up()
{
  if (_m_currentLayer->parent())
  {
    ScopeLayer* parent = _m_currentLayer->parent();
    delete _m_currentLayer;
    _m_currentLayer = parent;
  }
  
  return *_m_currentLayer;
}
