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

#ifndef __GPSE_CORE_OBJECT_IMPL_H__
#define __GPSE_CORE_OBJECT_IMPL_H__

#include "core/object.hpp"
#include "core/objectfactory.hpp"

namespace core
{
	template <typename T>
	Object::Object(T const& value)
	{ *this = ObjectFactory::build(value); }

	template <typename TRet, typename... TArgs>
	Object::Object(TRet(*function_ptr)(TArgs...))
	{ *this = ObjectFactory::build(function_ptr); }

    template <typename T>
    T& Object::unwrap()
    {
        if (m_kind != Kind::Scalar)
            throw std::runtime_error("attempt to unwrap a non-scalar object");

        if (!m_meta.is<T>())
            throw std::runtime_error("attempt to unwrap a object of incompatible type");

        return m_meta.as<T>();
    }

    template <typename T>
    T const& Object::unwrap() const
    {
        if (m_kind != Kind::Scalar)
            throw std::runtime_error("attempt to unwrap a non-scalar object");

        if (!m_meta.is<T>())
            throw std::runtime_error("attempt to unwrap a object of incompatible type");

        return m_meta.as<T>();
    }
}

#endif // __GPSE_CORE_OBJECT_IMPL_H__
