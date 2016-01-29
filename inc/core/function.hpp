#ifndef __GPSE_CORE_FUNCTION_H__
#define __GPSE_CORE_FUNCTION_H__

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
            ~Prototype();

            Type const& ret() const;
            void setRet(Type const& ret);

            std::vector<Variable>& args();
            std::vector<Variable> const& args() const;

        private:
            Type _m_ret;
            std::vector<Variable> _m_args;
        };

        class Function
        {
        public:
            Function();
            Function(std::string const& name, Prototype const& prototype);
            ~Function();

            std::string const& name() const;
            Prototype const& prototype() const;

            void setNode(lang::Node* node);
            lang::Node* node() const;

        private:
            std::string _m_name;
            Prototype _m_prototype;
            lang::Node* _m_node;
        };
    }
}

#endif // __GPSE_CORE_FUNCTION_H__
