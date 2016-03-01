#include "lang/node.hpp"
#include "lang/parser.hpp"

#include <algorithm>

using namespace gpse;
using namespace lang;

Node::Node(int which, Node* parent, Parser* parser)
    : m_which(which)
    , m_parser(parser)
    , m_parent(parent)
{
}

Node::~Node()
{
    for(auto it = m_children.begin(); it != m_children.end(); ++it)
    {
        delete *it;
    }
}

int Node::which() const
{
    return m_which;
}

void Node::setToken(Token const& token)
{
    m_token = token;
}

Token const& Node::token() const
{
    return m_token;
}

void Node::setParser(Parser* parser)
{
    m_parser = parser;
}

Parser* Node::parser() const
{
    return m_parser;
}

Node* Node::parent()
{
    return m_parent;
}

void Node::setParent(Node* parent)
{
    m_parent = parent;
}

std::vector<Node*>& Node::children()
{
    return m_children;
}

void Node::addChild(Node* child)
{
    child->setParent(this);
    m_children.push_back(child);
}

void Node::removeChild(Node* child)
{
    for(auto it = m_children.begin(); it != m_children.end(); ++it)
    {
        if(*it == child)
        {
            m_children.erase(it);
            break;
        }
    }
}

std::vector<Node*> const& Node::children() const
{
    return m_children;
}

void Node::substituteChild(Node* oldChild, Node* newChild)
{
    if(oldChild == newChild)
    {
        return;
    }

    auto it = std::find(m_children.begin(), m_children.end(), oldChild);

    if(it != m_children.end())
    {
        newChild->setParent(this);
        *it = newChild;
    }
}

void Node::error(std::string const& message) const
{
    if(m_parser)
    {
        m_parser->error(message, m_token);
    }
}
