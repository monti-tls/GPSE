#ifndef __GPSE_LANG_NODE_H__
#define __GPSE_LANG_NODE_H__

#include <vector>

namespace gpse
{
  namespace lang
  {
    class NodeVisitor;
    
    class Node
    {
    public:
      Node(int which, Node* parent);
      virtual ~Node();
      
      int which() const;
      
      Node* parent();
      void setParent(Node* parent);
      void addChild(Node* child);
      void removeChild(Node* child);
      std::vector<Node*>& children();
      std::vector<Node*> const& children() const;
      void substituteChild(Node* oldChild, Node* newChild);
      
      virtual void accept(NodeVisitor* visitor);
      
    private:
      int _m_which;
      Node* _m_parent;
      std::vector<Node*> _m_children;
    };
  }
}

#endif // __GPSE_LANG_NODE_H__
