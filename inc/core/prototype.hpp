#ifndef __GPSE_CORE_PROTOTYPE_H__
#define __GPSE_CORE_PROTOTYPE_H__

#include "core/type.hpp"
#include "core/variable.hpp"

#include <vector>
#include <string>

namespace gpse
{
    namespace lang
    {
        class Node;
    }

    namespace core
    {
        class Prototype
        {
        public:
            Prototype();
            Prototype(Type const& ret, std::vector<Variable> const& args = std::vector<Variable>());
            Prototype(Type const& ret, std::vector<Type> const& args_types = std::vector<Type>());
            ~Prototype();

            Type const& ret() const;
            void setRet(Type const& ret);

            std::vector<Variable>& args();
            std::vector<Variable> const& args() const;

        private:
            Type _m_ret;
            std::vector<Variable> _m_args;
        };
    }
}

#endif // __GPSE_CORE_PROTOTYPE_H__
