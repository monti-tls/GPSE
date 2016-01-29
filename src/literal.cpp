#include "core/literal.hpp"
#include <sstream>

using namespace gpse;
using namespace core;

Literal::Literal()
    : _m_value()
{
}

Literal::Literal(core::Some const& value)
    : _m_value(value)
{
}

Literal::Literal(bool boolean)
    : _m_value(boolean)
{
}

Literal::Literal(int integer)
    : _m_value(integer)
{
}

Literal::Literal(float floating)
    : _m_value(floating)
{
}

Literal::Literal(std::string const& string)
    : _m_value(string)
{
}

Literal::~Literal()
{
}

Literal& Literal::operator=(bool boolean)
{
    _m_value = boolean;
    return *this;
}

Literal& Literal::operator=(int integer)
{
    _m_value = integer;
    return *this;
}

Literal& Literal::operator=(float floating)
{
    _m_value = floating;
    return *this;
}

Literal& Literal::operator=(std::string const& string)
{
    _m_value = string;
    return *this;
}

core::Type Literal::type() const
{
    if(isInteger())
    {
        return core::Type::Integer;
    }
    else if(isFloating())
    {
        return core::Type::Floating;
    }
    else if(isString())
    {
        return core::Type::String;
    }
    else if(isBoolean())
    {
        return core::Type::Boolean;
    }

    return core::Type::Nil;
}

bool Literal::isNil() const
{
    return _m_value.empty();
}

bool Literal::isBoolean() const
{
    return _m_value.is<bool>();
}

bool Literal::isInteger() const
{
    return _m_value.is<int>();
}

bool Literal::isFloating() const
{
    return _m_value.is<float>();
}

bool Literal::isString() const
{
    return _m_value.is<std::string>();
}

core::Some const& Literal::value() const
{
    return _m_value;
}

bool Literal::boolean() const
{
    if(isNil())
    {
        return 0;
    }
    else if(isBoolean())
    {
        return _m_value.cast<bool>();
    }
    else if(isInteger())
    {
        return (bool)_m_value.cast<int>();
    }
    else if(isFloating())
    {
        return (bool)_m_value.cast<float>();
    }
    else if(isString())
    {
        std::istringstream ss(_m_value.cast<std::string>());
        bool value;
        ss >> value;
        return value;
    }

    return false;
}

int Literal::integer() const
{
    if(isNil())
    {
        return 0;
    }
    else if(isBoolean())
    {
        return (int)_m_value.cast<bool>();
    }
    else if(isInteger())
    {
        return _m_value.cast<int>();
    }
    else if(isFloating())
    {
        return (int)_m_value.cast<float>();
    }
    else if(isString())
    {
        std::istringstream ss(_m_value.cast<std::string>());
        int value;
        ss >> value;
        return value;
    }

    return 0;
}

float Literal::floating() const
{
    if(isNil())
    {
        return 0.0f;
    }
    else if(isBoolean())
    {
        return (float)_m_value.cast<bool>();
    }
    else if(isInteger())
    {
        return (float)_m_value.cast<int>();
    }
    else if(isFloating())
    {
        return _m_value.cast<float>();
    }
    else if(isString())
    {
        std::istringstream ss(_m_value.cast<std::string>());
        float value;
        ss >> value;
        return value;
    }

    return 0.0f;
}

std::string Literal::string() const
{
    if(isNil())
    {
        return "";
    }
    else if(isBoolean())
    {
        std::ostringstream ss;
        ss << _m_value.cast<bool>();
        return ss.str();
    }
    else if(isInteger())
    {
        std::ostringstream ss;
        ss << _m_value.cast<int>();
        return ss.str();
    }
    else if(isFloating())
    {
        std::ostringstream ss;
        ss << _m_value.cast<float>();
        return ss.str();
    }
    else if(isString())
    {
        return _m_value.cast<std::string>();
    }

    return "";
}

Literal Literal::operator-() const
{
    if(isInteger())
    {
        return Literal(-integer());
    }
    else if(isFloating())
    {
        return Literal(-floating());
    }
    else
    {
        return Literal();
    }
}

Literal Literal::operator!() const
{
    return Literal(!boolean());
}

Literal Literal::operator+(Literal const& rhs) const
{
    Literal lit;

    if(isFloating())
    {
        lit = floating() + rhs.floating();
    }
    else
    {
        lit = integer() + rhs.integer();
    }

    return lit;
}

Literal Literal::operator-(Literal const& rhs) const
{
    Literal lit;

    if(isFloating())
    {
        lit = floating() - rhs.floating();
    }
    else
    {
        lit = integer() - rhs.integer();
    }

    return lit;
}

Literal Literal::operator*(Literal const& rhs) const
{
    Literal lit;

    if(isFloating())
    {
        lit = floating() * rhs.floating();
    }
    else
    {
        lit = integer() * rhs.integer();
    }

    return lit;
}

Literal Literal::operator/(Literal const& rhs) const
{
    Literal lit;

    if(isFloating())
    {
        lit = floating() / rhs.floating();
    }
    else
    {
        lit = integer() / rhs.integer();
    }

    return lit;
}

Literal Literal::operator||(Literal const& rhs) const
{
    Literal lit;

    lit = boolean() || rhs.boolean();

    return lit;
}

Literal Literal::operator&&(Literal const& rhs) const
{
    Literal lit;

    lit = boolean() && rhs.boolean();

    return lit;
}

Literal Literal::operator<(Literal const& rhs) const
{
    Literal lit;

    if(isFloating())
    {
        lit = floating() < rhs.floating();
    }
    else
    {
        lit = integer() < rhs.integer();
    }

    return lit;
}

Literal Literal::operator<=(Literal const& rhs) const
{
    Literal lit;

    if(isFloating())
    {
        lit = floating() <= rhs.floating();
    }
    else
    {
        lit = integer() <= rhs.integer();
    }

    return lit;
}

Literal Literal::operator>(Literal const& rhs) const
{
    Literal lit;

    if(isFloating())
    {
        lit = floating() > rhs.floating();
    }
    else
    {
        lit = integer() > rhs.integer();
    }

    return lit;
}

Literal Literal::operator>=(Literal const& rhs) const
{
    Literal lit;

    if(isFloating())
    {
        lit = floating() >= rhs.floating();
    }
    else
    {
        lit = integer() >= rhs.integer();
    }

    return lit;
}

Literal Literal::operator==(Literal const& rhs) const
{
    Literal lit;
    if(isNil())
    {
        lit = true;
    }
    else if(isFloating())
    {
        lit = floating() == rhs.floating();
    }
    else if(isInteger())
    {
        lit = integer() == rhs.integer();
    }
    else if(isString())
    {
        lit = string() == rhs.string();
    }
    else if(isBoolean())
    {
        lit = boolean() == rhs.boolean();
    }

    return lit;
}

Literal Literal::operator!=(Literal const& rhs) const
{
    return !operator==(rhs);
}
