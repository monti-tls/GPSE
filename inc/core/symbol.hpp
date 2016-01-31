#ifndef __GPSE_CORE_SYMBOL_H__
#define __GPSE_CORE_SYMBOL_H__

#include "core/some.hpp"
#include "core/variable.hpp"
#include "core/type.hpp"
#include "core/function.hpp"
#include "core/callback.hpp"
#include "core/symbol.hpp"

namespace gpse
{
    namespace core
    {
        class Symbol
        {
        public:
            Symbol();
            Symbol(Variable const& variable);
            Symbol(Type const& type);
            Symbol(Function const& function);
            Symbol(Callback const& callback);
            ~Symbol();

            bool isVariable() const;
            bool isType() const;
            bool isFunction() const;
            bool isCallback() const;

            Variable& variable();
            Variable const& variable() const;

            Type& type();
            Type const& type() const;

            Function& function();
            Function const& function() const;

            Callback& callback();
            Callback const& callback() const;

        private:
            Some _m_value;
        };
    }
}

#endif // __GPSE_CORE_SYMBOL_H__
