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

#ifndef __GPSE_CORE_OBJECT_H__
#define __GPSE_CORE_OBJECT_H__

#include "core/some.hpp"

#include <vector>
#include <string>
#include <map>

namespace core
{
    class Object
    {
    public:
        enum class Kind
        {
            Nil,
            Callable,
            Scalar
        };

    public:
        template <typename T>
        Object(T const& value);

        template <typename TRet, typename... TArgs>
        Object(TRet(*function_ptr)(TArgs...));

        Object(Kind kind = Kind::Nil, Some meta = Some());
        ~Object();

        Kind kind() const;
        Some const& meta() const;
        bool has(std::string const& id) const;
        Object& member(std::string const& id);
        Object const& member(std::string const& id) const;
        Object invoke(std::vector<Object> const& args) const;

        template <typename T>
        T& unwrap();

        template <typename T>
        T const& unwrap() const;

        Object operator()(std::vector<Object> const& args = {}) const;
        Object operator+(Object const& other) const;
        Object operator-(Object const& other) const;
        Object operator*(Object const& other) const;
        Object operator/(Object const& other) const;
        Object operator%(Object const& other) const;

    public:
        static Object Nil;

    private:
        Kind m_kind;
        Some m_meta;
        std::map<std::string, Object> m_members;
    };
}

#endif // __GPSE_CORE_OBJECT_H__
