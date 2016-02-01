#ifndef __GPSE_CORE_LITERAL_H__
#define __GPSE_CORE_LITERAL_H__

#include "core/some.hpp"
#include "core/type.hpp"

#include <string>

namespace gpse
{
    namespace core
    {
        class Literal
        {
        public:
            Literal();
            Literal(Some const& value);
            Literal(bool boolean);
            Literal(int integer);
            Literal(float floating);
            Literal(std::string const& string);
            ~Literal();

            Literal& operator=(bool boolean);
            Literal& operator=(int integer);
            Literal& operator=(float floating);
            Literal& operator=(std::string const& string);

            Type type() const;
            bool is(core::Type const& type) const;
            bool isNil() const;
            bool isBoolean() const;
            bool isInteger() const;
            bool isFloating() const;
            bool isString() const;

            Some const& value() const;
            Some cast(core::Type const& type) const;
            bool boolean() const;
            int integer() const;
            float floating() const;
            std::string string() const;

            Literal operator-() const;
            Literal operator!() const;

            Literal operator+(Literal const& rhs) const;
            Literal operator-(Literal const& rhs) const;
            Literal operator*(Literal const& rhs) const;
            Literal operator/(Literal const& rhs) const;
            Literal operator&&(Literal const& rhs) const;
            Literal operator||(Literal const& rhs) const;
            Literal operator<(Literal const& rhs) const;
            Literal operator<=(Literal const& rhs) const;
            Literal operator>(Literal const& rhs) const;
            Literal operator>=(Literal const& rhs) const;
            Literal operator==(Literal const& rhs) const;
            Literal operator!=(Literal const& rhs) const;

        private:
            Some _m_value;
        };
    }
}

#endif // __GPSE_CORE_LITERAL_H__
