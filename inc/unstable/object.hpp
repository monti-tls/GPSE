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

#endif // __GPSE_UNSTABLE_OBJECT_H__
