#ifndef __GPSE_UNSTABLE_CALLBACK_IMPL_H__
#define __GPSE_UNSTABLE_CALLBACK_IMPL_H__

namespace unstable
{
    template <typename TRet, typename... TArgs>
    Object CallbackImpl<TRet, TArgs...>::invoke(std::vector<Object> const& args)
    {
        std::tuple<TArgs...> tp = this->M_buildTuple(args.begin(), (TArgs*)nullptr...);

        return ObjectFactory::build(apply(this->_m_fun, tp));
    }

    template <typename... TArgs>
    Object CallbackImpl<void, TArgs...>::invoke(std::vector<Object> const& args)
    {
        std::tuple<TArgs...> tp = this->M_buildTuple(args.begin(), (TArgs*)nullptr...);
        apply(this->_m_fun, tp);

        return Object::Nil;
    }
}

#endif // __GPSE_UNSTABLE_CALLBACK_IMPL_H__
