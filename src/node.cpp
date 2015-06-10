#include "lang/node.hpp"

#include <algorithm>

using namespace gpse;
using namespace lang;

Node::Node(int which, Node* parent):
  _m_which(which),
  _m_parent(parent)
{
}

Node::~Node()
{
  for (auto it = _m_children.begin(); it != _m_children.end(); ++it)
  {
    delete *it;
  }
}

int Node::which() const
{
  return _m_which;
}

Node* Node::parent()
{
  return _m_parent;
}

void Node::setParent(Node* parent)
{
  _m_parent = parent;
}

std::vector<Node*>& Node::children()
{
  return _m_children;
}

void Node::addChild(Node* child)
{
  child->setParent(this);
  _m_children.push_back(child);
}

void Node::removeChild(Node* child)
{
  for (auto it = _m_children.begin(); it != _m_children.end(); ++it)
  {
    if (*it == child)
    {
      _m_children.erase(it);
      break;
    }
  }
}

std::vector<Node*> const& Node::children() const
{
  return _m_children;
}

void Node::substituteChild(Node* oldChild, Node* newChild)
{
  if (oldChild == newChild)
  {
    return;
  }
  
  auto it = std::find(_m_children.begin(), _m_children.end(), oldChild);
  
  if (it != _m_children.end())
  {
    newChild->setParent(this);
    *it = newChild;
  }
}
