#ifndef __CORE_VARIABLE_NAME_H__
#define __CORE_VARIABLE_NAME_H__

#include "core/type.hpp"
#include "core/some.hpp"

#include <string>

namespace gpse
{
    namespace core
    {
        class Variable
        {
        public:
            Variable();
            Variable(std::string const& name, core::Type const& type);
            ~Variable();

            void setName(std::string const& name);
            std::string const& name() const;

            void setType(core::Type const& type);
            core::Type const& type() const;

            void setValue(core::Some const& value);
            core::Some const& value() const;

        private:
            std::string _m_name;
            core::Type _m_type;

            core::Some _m_value;
        };
    }
}

#endif // __CORE_VARIABLE_NAME_H__
