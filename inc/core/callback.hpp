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

#ifndef __GPSE_CORE_CALLBACK_H__
#define __GPSE_CORE_CALLBACK_H__

#include "core/object.hpp"

#include <vector>
#include <string>
#include <functional>
#include <cstddef>
#include <type_traits>
#include <utility>
#include <tuple>
#include <memory>

namespace core
{
    template <size_t N>
    struct Apply
    {
        template <typename F, typename T, typename... A>
        static inline auto apply(F&& f, T&& t, A&&... a) -> decltype(Apply<N - 1>::apply(::std::forward<F>(f), ::std::forward<T>(t), ::std::get<N - 1>(::std::forward<T>(t)), ::std::forward<A>(a)...))
        {
            return Apply<N - 1>::apply(::std::forward<F>(f), ::std::forward<T>(t), ::std::get<N - 1>(::std::forward<T>(t)), ::std::forward<A>(a)...);
        }
    };

    template <>
    struct Apply<0>
    {
        template <typename F, typename T, typename... A>
        static inline auto apply(F&& f, T&&, A&&... a) -> decltype(::std::forward<F>(f)(::std::forward<A>(a)...))
        {
            return ::std::forward<F>(f)(::std::forward<A>(a)...);
        }
    };

    template <typename F, typename T>
    inline auto apply(F&& f, T&& t) -> decltype(Apply<::std::tuple_size<typename ::std::decay<T>::type>::value>::apply(::std::forward<F>(f), ::std::forward<T>(t)))
    {
        return Apply<::std::tuple_size<typename ::std::decay<T>::type>::value>::apply(::std::forward<F>(f), ::std::forward<T>(t));
    }

    class AbstractCallback
    {
    public:
        AbstractCallback()
        {
        }
        virtual ~AbstractCallback()
        {
        }

        virtual Object invoke(std::vector<Object> const& args) = 0;
    };

    template <typename TRet, typename... TArgs>
    class CallbackBase : public AbstractCallback
    {
    public:
        CallbackBase(std::function<TRet(TArgs...)> const& fun)
            : m_fun(fun)
        {
        }

        ~CallbackBase()
        {
        }

    protected:
        template <typename A, typename... Args>
        std::tuple<A, Args...> M_buildTuple(std::vector<Object>::const_iterator it, A*, Args*... args)
        {
            std::tuple<A> first = std::make_tuple(it->unwrap<A>());
            std::tuple<Args...> second = this->M_buildTuple(++it, args...);
            return std::tuple_cat(first, second);
        }

        std::tuple<> M_buildTuple(std::vector<Object>::const_iterator it)
        {
            return std::tuple<>();
        }

    protected:
        std::function<TRet(TArgs...)> m_fun;
    };

    template <typename TRet, typename... TArgs>
    class CallbackImpl : public CallbackBase<TRet, TArgs...>
    {
    public:
        using CallbackBase<TRet, TArgs...>::CallbackBase;

        Object invoke(std::vector<Object> const& args);
    };

    template <typename... TArgs>
    class CallbackImpl<void, TArgs...> : public CallbackBase<void, TArgs...>
    {
    public:
        using CallbackBase<void, TArgs...>::CallbackBase;

        Object invoke(std::vector<Object> const& args);
    };
}

#endif // __GPSE_CORE_CALLBACK_H__
