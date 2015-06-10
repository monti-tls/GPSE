#ifndef __GPSE_CORE_NAMED_SCOPE_H__
#define __GPSE_CORE_NAMED_SCOPE_H__

#include <vector>
#include <map>

namespace gpse
{
  namespace core
  {
    template <typename K, typename T>
    class NamedScope
    {
    public:
      NamedScope(NamedScope<K, T>* parent = nullptr, NamedScope<K, T>* child = nullptr) :
        _m_parent(parent),
        _m_child(child)
      {}
      
      ~NamedScope()
      {
        delete _m_child;
      }
      
      void setParent(NamedScope<K, T>* parent)
      {
        _m_parent = parent();
      }
      
      NamedScope<K, T>* parent() const
      {
        return _m_parent;
      }
      
      void setChild(NamedScope<K, T>* child)
      {
        _m_child = child;
      }
      
      NamedScope<K, T>* child() const
      {
        return _m_child;
      }
      
      void addElement(K const& name, T const& element)
      {
        _m_content[name] = element;
      }
      
      bool scopedFind(K const& name, T* element = nullptr)
      {
        auto it = _m_content.find(name);
        if (it == _m_content.end())
        {
          return false;
        }
        
        if (element)
        {
          *element = it->second;
        }
        
        return true;
      }
      
      bool find(K const& name, T* element = nullptr)
      {
        auto it = _m_content.find(name);
        if (it == _m_content.end())
        {
          if (_m_parent)
          {
            return _m_parent->find(name, element);
          }
          
          return false;
        }
        
        if (element)
        {
          *element = it->second;
        }
        
        return true;
      }
      
    private:
      NamedScope<K, T>* _m_parent;
      NamedScope<K, T>* _m_child;
      std::map<K, T> _m_content;
    };
  }
}

#endif // __GPSE_CORE_NAMED_SCOPE_H__
