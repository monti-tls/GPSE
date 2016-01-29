#ifndef __GPSE_LANG_TREE_PASS_H__
#define __GPSE_LANG_TREE_PASS_H__

#include "lang/node.hpp"
#include "core/some.hpp"

#include <functional>
#include <map>
#include <string>

namespace gpse
{
    namespace lang
    {
        class TreePass
        {
        public:
            typedef std::function<void(Node*&)> operator_t;

        public:
            TreePass();
            ~TreePass();

            template <int which, typename T>
            void addOperator(typename std::function<void(TreePass*, Node*&, T*)> const& op)
            {
                _m_ops[which] = [=](Node*& node)
                {
                    op(this, node, static_cast<T*>(node));
                };
            }

            void setDefaultOperator(std::function<void(TreePass*, Node*&)> const& op);
            void pass(Node*& node);

            std::map<std::string, TreePass>& subPasses();
            std::map<std::string, TreePass> const& subPasses() const;
            core::Some& storage();
            core::Some const& storage() const;

        private:
            operator_t _m_defaultOp;
            std::map<int, operator_t> _m_ops;
            std::map<std::string, TreePass> _m_subpasses;
            core::Some _m_storage;
        };
    }
}

#endif // __GPSE_LANG_TREE_PASS_H__
