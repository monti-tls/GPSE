#ifndef __GPSE_LANG_NODE_H__
#define __GPSE_LANG_NODE_H__

#include "lang/token.hpp"

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

            Node* parent();
            void setParent(Node* parent);
            void addChild(Node* child);
            void removeChild(Node* child);
            std::vector<Node*>& children();
            std::vector<Node*> const& children() const;
            void substituteChild(Node* oldChild, Node* newChild);

            void error(std::string const& message) const;

        private:
            int m_which;
            lang::Token m_token;
            Parser* m_parser;
            Node* m_parent;
            std::vector<Node*> m_children;
        };
    }
}

#endif // __GPSE_LANG_NODE_H__
