#ifndef __GPSE_LANG_NODE_VISITOR_H__
#define __GPSE_LANG_NODE_VISITOR_H__

#include "lang/node.hpp"

namespace gpse
{
  namespace lang
  {
    class NodeVisitor
    {
    public:
      NodeVisitor();
      virtual ~NodeVisitor();
      
      virtual void visit(Node* node) = 0;
    };
  }
}

#endif // __GPSE_LANG_NODE_VISITOR_H__
