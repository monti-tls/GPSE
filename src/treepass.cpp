#include "lang/treepass.hpp"

using namespace gpse;
using namespace lang;

TreePass::TreePass() :
  _m_defaultOp()
{
  _m_defaultOp =
  [=](Node*& node)
  {
    for (Node*& child : node->children())
    {
      this->pass(child);
    }
  };
}

TreePass::~TreePass()
{
  
}

std::map<std::string, TreePass>& TreePass::subPasses()
{
  return _m_subpasses;
}

std::map<std::string, TreePass> const& TreePass::subPasses() const
{
  return _m_subpasses;
}

core::Some& TreePass::storage()
{
  return _m_storage;
}

core::Some const& TreePass::storage() const
{
  return _m_storage;
}

void TreePass::setDefaultOperator(std::function<void (TreePass*, Node*&)> const& op)
{
  _m_defaultOp = [=](Node*& node)
  {
    op(this, node);
  };
}

void TreePass::pass(Node*& node)
{
  auto it = _m_ops.find(node->which());
  if (it != _m_ops.end())
  {
    (it->second)(node);
  }
  else
  {
    _m_defaultOp(node);
  }
}
