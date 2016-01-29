#ifndef __GPSE_LANG_NODE_H__
#define __GPSE_LANG_NODE_H__

#include "lang/token.hpp"
#include "core/scope.hpp"

#include <vector>

namespace gpse
{
    namespace lang
    {
        class Parser;

        class Node
        {
        public:
            Node(int which, Node* parent, Parser* parser = nullptr);
            virtual ~Node();

            int which() const;
            void setToken(Token const& token);
            Token const& token() const;
            void setParser(Parser* parser);
            Parser* parser() const;
            void setScope(core::Scope* scope);
            core::Scope* scope() const;
            void setScopeLayer(core::ScopeLayer* layer);
            core::ScopeLayer* scopeLayer() const;

            Node* parent();
            void setParent(Node* parent);
            void addChild(Node* child);
            void removeChild(Node* child);
            std::vector<Node*>& children();
            std::vector<Node*> const& children() const;
            void substituteChild(Node* oldChild, Node* newChild);

            void error(std::string const& message) const;

        private:
            int _m_which;
            lang::Token _m_token;
            Parser* _m_parser;
            Node* _m_parent;
            core::Scope* _m_scope;
            core::ScopeLayer* _m_scopeLayer;
            std::vector<Node*> _m_children;
        };
    }
}

#endif // __GPSE_LANG_NODE_H__
