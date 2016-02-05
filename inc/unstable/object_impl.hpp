#ifndef __GPSE_UNSTABLE_OBJECT_IMPL_H__
#define __GPSE_UNSTABLE_OBJECT_IMPL_H__

#include "unstable/object.hpp"
#include "unstable/objectfactory.hpp"

namespace unstable
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

#endif // __GPSE_UNSTABLE_OBJECT_IMPL_H__
