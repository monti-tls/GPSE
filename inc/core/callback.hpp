#ifndef __GPSE_CORE_CALLBACK_H__
#define __GPSE_CORE_CALLBACK_H__

#include "core/type.hpp"
#include "core/variable.hpp"
#include "core/function.hpp"

#include <vector>
#include <string>
#include <functional>
#include <cstddef>
#include <type_traits>
#include <utility>
#include <tuple>
#include <memory>

namespace gpse
{
    namespace core
    {
        namespace detail
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

            template <typename T>
            inline core::Type whichType()
            {
                return core::Type::Opaque;
            }

            template <>
            inline core::Type whichType<void>()
            {
                return core::Type::Nil;
            }

            template <>
            inline core::Type whichType<int>()
            {
                return core::Type::Integer;
            }

            template <>
            inline core::Type whichType<float>()
            {
                return core::Type::Floating;
            }

            template <>
            inline core::Type whichType<std::string>()
            {
                return core::Type::String;
            }

            template <>
            inline core::Type whichType<bool>()
            {
                return core::Type::Boolean;
            }

            //! The abstract base class for C++ callbacks
            class AbstractCallback
            {
            public:
                AbstractCallback()
                {
                }
                virtual ~AbstractCallback()
                {
                }

                //! This abstract function effectively invokes the callback
                //! \param args The argument vector, dynamically casted when called
                //! \return The eventual return value of the callback (empty gpse::core::Some
                //!         if it returns void)
                virtual core::Some call(std::vector<core::Some> const& args) = 0;

                //! This abstract function returns the prototype of the callback
                //! \return The prototype instance of the class
                virtual core::Prototype prototype() const = 0;
            };

            //! This class implements basic callback primitives, like
            //!   generating the prototype from the variadic template
            //!   C++ function signature, and unpacking the argument vector
            //!   sequence to a casted tuple.
            template <typename TRet, typename... TArgs>
            class CallbackBase : public AbstractCallback
            {
            public:
                //! Construct a callback base containing a standard
                //!   function wrapper
                //! \param fun The function to integrate in the callback system
                CallbackBase(std::function<TRet(TArgs...)> const& fun)
                    : _m_fun(fun)
                {
                }

                ~CallbackBase()
                {
                }

                //! This function returns the generated function prototype
                //!   reflecting this callback's C++ signature.
                //! Non-scalar types are viewed as gpse::core::Type::Opaque types.
                //! \return The generated prototype
                core::Prototype prototype() const
                {
                    return core::Prototype(whichType<TRet>(), _M_argumentTypes((TArgs*)nullptr...));
                }

            protected:
                template <typename A, typename... Args>
                std::tuple<A, Args...> _M_call(std::vector<core::Some>::const_iterator it, A*, Args*... args)
                {
                    std::tuple<A> first = std::make_tuple(it->as<A>());
                    std::tuple<Args...> second = this->_M_call(++it, args...);
                    return std::tuple_cat(first, second);
                }

                std::tuple<> _M_call(std::vector<core::Some>::const_iterator it)
                {
                    return std::tuple<>();
                }

                template <typename A, typename... Args>
                std::vector<core::Type> _M_argumentTypes(A*, Args*... args) const
                {
                    std::vector<core::Type> vec = this->_M_argumentTypes(args...);
                    vec.push_back(whichType<A>());

                    return vec;
                }

                std::vector<core::Type> _M_argumentTypes() const
                {
                    return std::vector<core::Type>();
                }

            protected:
                std::function<TRet(TArgs...)> _m_fun;
            };

            //! This class implements a callback for
            //!   functions not returning void.
            template <typename TRet, typename... TArgs>
            class CallbackImpl : public CallbackBase<TRet, TArgs...>
            {
            public:
                using CallbackBase<TRet, TArgs...>::CallbackBase;

                //! Using gpse::core::detail::apply(), this function effectively calls
                //!   the enclosed function using the generated casted tuple as arguments.
                //! \param args The wrapped arguments vector
                //! \return The wrapped function's return value
                core::Some call(std::vector<core::Some> const& args)
                {
                    std::tuple<TArgs...> tp = this->_M_call(args.begin(), (TArgs*)nullptr...);
                    return core::Some(apply(this->_m_fun, tp));
                }
            };

            //! This class implements a callback for
            //!   functions returning void.
            template <typename... TArgs>
            class CallbackImpl<void, TArgs...> : public CallbackBase<void, TArgs...>
            {
            public:
                using CallbackBase<void, TArgs...>::CallbackBase;

                //! Using gpse::core::detail::apply(), this function effectively calls
                //!   the enclosed function using the generated casted tuple as arguments.
                //! \param args The wrapped arguments vector
                //! \return An empty value
                core::Some call(std::vector<core::Some> const& args)
                {
                    std::tuple<TArgs...> tp = this->_M_call(args.begin(), (TArgs*)nullptr...);
                    apply(this->_m_fun, tp);

                    return core::Some();
                }
            };
        }

        //! This is the callback structure viewed as a symbol,
        //!   like gpse::core::Function or gpse::core::Variable.
        class Callback
        {
        public:
            //! Create a callback from a function pointer.
            //! \param fun The function pointer to enclose in this callback structure
            //! \param name The name of the callback symbol
            template <typename TRet, typename... TArgs>
            Callback(std::function<TRet(TArgs...)> const& fun, std::string const& name)
                : _m_impl(new detail::CallbackImpl<TRet, TArgs...>(fun))
                , _m_name(name)
            {
            }

            ~Callback()
            {
            }

            //! Invoke this callback object using a vector of
            //!   wrapped values as arguments
            //! \param args The argument vector as wrapped values
            //! \return The wrapped return value of the call,
            //!         empty if it returns void
            core::Some call(std::vector<core::Some> const& args) const;

            //! Get the prototype of this callback
            //! \return The prototype of this callback
            core::Prototype prototype() const;

            //! Get the name of this callback symbol
            //! \return The name of this callback symbol
            std::string const& name() const;

        private:
            std::shared_ptr<detail::AbstractCallback> _m_impl;
            std::string _m_name;
        };
    }
}

#endif // __GPSE_CORE_CALLBACK_H__
