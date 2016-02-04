#ifndef __GPSE_UNSTABLE_OBJECT_H__
#define __GPSE_UNSTABLE_OBJECT_H__

#include "unstable/some.hpp"

#include <vector>
#include <string>
#include <map>

namespace unstable
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
        Object(Kind kind = Kind::Nil, Some meta = Some());
        ~Object();

        Kind kind() const;
        Some const& meta() const;
        bool has(std::string const& id) const;
        Object& member(std::string const& id);
        Object const& member(std::string const& id) const;
        Object invoke(std::vector<Object> const& args) const;

        template <typename T>
        T& unwrap()
        {
            if (m_kind != Kind::Scalar)
                throw std::runtime_error("attempt to unwrap a non-scalar object");

            if (!m_meta.is<T>())
                throw std::runtime_error("attempt to unwrap a object of incompatible type");

            return m_meta.as<T>();
        }

        template <typename T>
        T const& unwrap() const
        {
            if (m_kind != Kind::Scalar)
                throw std::runtime_error("attempt to unwrap a non-scalar object");

            if (!m_meta.is<T>())
                throw std::runtime_error("attempt to unwrap a object of incompatible type");

            return m_meta.as<T>();
        }

    public:
        static Object Nil;

    private:
        Kind m_kind;
        Some m_meta;
        std::map<std::string, Object> m_members;
    };
}

#endif // __GPSE_UNSTABLE_OBJECT_H__
