#ifndef __GPSE_CORE_SCOPE_H__
#define __GPSE_CORE_SCOPE_H__

#include "core/symbol.hpp"

#include <vector>
#include <map>

namespace gpse
{
  namespace core
  {
    class ScopeLayer
    {
    public:
      ScopeLayer(ScopeLayer* parent = nullptr, ScopeLayer* child = nullptr);
      ~ScopeLayer();
      
      void setParent(ScopeLayer* parent);
      ScopeLayer* parent() const;
      void setChild(ScopeLayer* child);
      ScopeLayer* child() const;
      
      void addElement(std::string const& name, Symbol const& element);
      bool find(std::string const& name, Symbol* element = nullptr);
      
    private:
      ScopeLayer* _m_parent;
      ScopeLayer* _m_child;
      std::map<std::string, Symbol> _m_content;
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
