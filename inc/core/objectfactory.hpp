/*  This file is part of gpse.
 *
 * gpse is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * gpse is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with gpse.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __GPSE_CORE_OBJECTFACTORY_H__
#define __GPSE_CORE_OBJECTFACTORY_H__

#include "core/some.hpp"
#include "core/object.hpp"
#include "core/callable.hpp"

#include <string>
#include <map>
#include <functional>

namespace core
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

#endif // __GPSE_CORE_OBJECTFACTORY_H__
