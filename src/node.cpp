#include "lang/node.hpp"
#include "lang/parser.hpp"

#include <algorithm>

using namespace gpse;
using namespace lang;

Node::Node(int which, Node* parent, Parser* parser)
    : _m_which(which)
    , _m_parser(parser)
    , _m_parent(parent)
    , _m_scope(nullptr)
    , _m_scopeLayer(nullptr)
{
}

Node::~Node()
{
    for(auto it = _m_children.begin(); it != _m_children.end(); ++it)
    {
        delete *it;
    }
}

int Node::which() const
{
    return _m_which;
}

void Node::setToken(Token const& token)
{
    _m_token = token;
}

Token const& Node::token() const
{
    return _m_token;
}

void Node::setParser(Parser* parser)
{
    _m_parser = parser;
}

Parser* Node::parser() const
{
    return _m_parser;
}

void Node::setScope(core::Scope* scope)
{
    _m_scope = scope;
}

core::Scope* Node::scope() const
{
    return _m_scope;
}

void Node::setScopeLayer(core::ScopeLayer* layer)
{
    _m_scopeLayer = layer;
}

core::ScopeLayer* Node::scopeLayer() const
{
    return _m_scopeLayer;
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
    for(auto it = _m_children.begin(); it != _m_children.end(); ++it)
    {
        if(*it == child)
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
    if(oldChild == newChild)
    {
        return;
    }

    auto it = std::find(_m_children.begin(), _m_children.end(), oldChild);

    if(it != _m_children.end())
    {
        newChild->setParent(this);
        *it = newChild;
    }
}

void Node::error(std::string const& message) const
{
    if(_m_parser)
    {
        _m_parser->error(message, _m_token);
    }
}
