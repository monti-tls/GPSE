#ifndef __GPSE_UNSTABLE_CALLABLE_H__
#define __GPSE_UNSTABLE_CALLABLE_H__

#include "unstable/some.hpp"
#include "unstable/object.hpp"
#include "unstable/callback.hpp"

#include <functional>
#include <vector>
#include <memory>

namespace unstable
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

#endif // __GPSE_UNSTABLE_CALLABLE_H__
