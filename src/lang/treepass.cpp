#include "lang/treepass.hpp"

using namespace gpse;
using namespace lang;

TreePass::TreePass()
    : m_defaultOp()
{
    m_defaultOp = [=](Node*& node)
    {
        for(Node*& child : node->children())
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
    return m_subpasses;
}

std::map<std::string, TreePass> const& TreePass::subPasses() const
{
    return m_subpasses;
}

core::Some& TreePass::storage()
{
    return m_storage;
}

core::Some const& TreePass::storage() const
{
    return m_storage;
}

void TreePass::setDefaultOperator(std::function<void(TreePass*, Node*&)> const& op)
{
    m_defaultOp = [=](Node*& node)
    {
        op(this, node);
    };
}

void TreePass::pass(Node*& node)
{
    auto it = m_ops.find(node->which());
    if(it != m_ops.end())
    {
        (it->second)(node);
    }
    else
    {
        m_defaultOp(node);
    }
}
