#ifndef __GPSE_UNSTABLE_OBJECTFACTORY_H__
#define __GPSE_UNSTABLE_OBJECTFACTORY_H__

#include "unstable/some.hpp"
#include "unstable/object.hpp"
#include "unstable/callable.hpp"

#include <string>
#include <map>
#include <functional>

namespace unstable
{
    namespace detail
    {
        template <typename T>
        struct Helper
        { static void helper() {} };
    }

    template <typename T>
    static inline size_t typeId()
    { return (size_t) &detail::Helper<T>::helper; }

    class ObjectFactory
    {
    public:
        typedef std::function<Object(Some const&)> Constructor;

    public:
        template <typename T>
        static void registerType(Object(*constructor)(T const&))
        { m_constructors[typeId<T>()] = [=](Some const& value) -> Object { return constructor(value.as<T>()); }; }

        template <typename T>
        static Object build(T const& value)
        {
            std::map<size_t, Constructor>::iterator it = m_constructors.find(typeId<T>());
            if (it == m_constructors.end())
                throw std::runtime_error("object type is not registered");

            return it->second(Some(value));
        }

        template <typename TRet, typename... TArgs>
        static Object build(TRet(*function_ptr)(TArgs...))
        {
            return Object(Object::Kind::Callable, Callable(function_ptr));
        }

    private:
        static std::map<size_t, Constructor> m_constructors;
    };
}

#endif // __GPSE_UNSTABLE_OBJECTFACTORY_H__
