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

#ifndef __GPSE_CORE_CALLABLE_H__
#define __GPSE_CORE_CALLABLE_H__

#include "core/some.hpp"
#include "core/object.hpp"
#include "core/callback.hpp"

#include <functional>
#include <vector>
#include <memory>

namespace core
{
    class Callable
    {
    public:
        enum class Kind
        {
            Native,
            Scripted
        };

        typedef std::shared_ptr<AbstractCallback> NativeMetaType;

    public:
        /* Callable(lang::Node* ast) {} */

        template <typename TRet, typename... TArgs>
        Callable(TRet(*function_ptr)(TArgs...))
            : m_kind(Kind::Native)
            , m_meta(NativeMetaType(new CallbackImpl<TRet, TArgs...>(std::function<TRet(TArgs...)>(function_ptr))))
        {}

        ~Callable();

        Object invoke(std::vector<Object> const& args) const;

    private:
        Object M_nativeInvocation(std::vector<Object> const& args) const;
        Object M_scriptedInvocation(std::vector<Object> const& args) const;

    private:
        Kind m_kind;
        Some m_meta;
    };
}

#endif // __GPSE_CORE_CALLABLE_H__
