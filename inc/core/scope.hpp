#ifndef __GPSE_CORE_SCOPE_H__
#define __GPSE_CORE_SCOPE_H__

#include <vector>
#include <algorithm>

namespace gpse
{
  namespace core
  {
    template <typename T>
    class Scope
    {
    public:
      Scope(Scope<T>* parent = nullptr, Scope<T>* child = nullptr) :
        _m_parent(parent),
        _m_child(child)
      {}
      
      ~Scope()
      {
        delete _m_child;
      }
      
      Scope<T>* parent() const
      {
        return _m_parent;
      }
      
      void setParent(Scope<T>* parent)
      {
        _m_parent = parent;
      }
      
      Scope<T>* child() const
      {
        return _m_child;
      }
      
      void setChild(Scope<T>* child)
      {
        _m_child = child;
      }
      
      void addElement(T const& element)
      {
        _m_content.push_back(element);
      }
      
      bool scopedFind(T const& element)
      {
        auto it = std::find(_m_content.begin(), _m_content.end(), element);
        if (it == _m_content.end())
        {
          return false;
        }
        
        return true;
      }
      
      bool find(T const& element)
      {
        auto it = _m_content.find(element);
        if (it == _m_content.end())
        {
          if (_m_parent)
          {
            return _m_parent->find(element);
          }
          
          return false;
        }
        
        return true;
      }
      
    private:
      Scope<T>* _m_parent;
      Scope<T>* _m_child;
      std::vector<Scope*> _m_children;
      std::vector<T> _m_content;
    };
  }
}

#endif // __GPSE_CORE_SCOPE_H__
