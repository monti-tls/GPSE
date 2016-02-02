#ifndef __GPSE_CORE_SCOPE_H__
#define __GPSE_CORE_SCOPE_H__

#include "core/symbol.hpp"

#include <vector>
#include <map>
#include <stack>

namespace gpse
{
    namespace core
    {
        class ScopeLayer
        {
        public:
            ScopeLayer(ScopeLayer* parent = nullptr);
            ~ScopeLayer();

            void setParent(ScopeLayer* parent);
            ScopeLayer* parent() const;
            std::vector<ScopeLayer*>& children();
            std::vector<ScopeLayer*> const& children() const;

            void addElement(std::string const& name, Symbol const& element);
            bool find(std::string const& name, Symbol* element = nullptr);
            bool findInScope(std::string const& name, Symbol* element = nullptr);

            Symbol* findRef(std::string const& name);
            Symbol* findRefInScope(std::string const& name);

            void preserve();
            void restore();

        private:
            ScopeLayer* _m_parent;
            std::vector<ScopeLayer*> _m_children;

            std::stack<std::map<std::string, Symbol>> _m_content;
        };

        class Scope
        {
        public:
            Scope();
            ~Scope();

            ScopeLayer& layer();
            ScopeLayer const& layer() const;
            ScopeLayer& down();
            ScopeLayer& up();

        private:
            ScopeLayer* _m_currentLayer;
        };
    }
}

#endif // __GPSE_CORE_SCOPE_H__
