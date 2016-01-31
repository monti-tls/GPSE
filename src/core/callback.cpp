#include "core/callback.hpp"

using namespace gpse;
using namespace core;
using namespace detail;

template <typename T>
            const core::Type WhichType<T>::type = core::Type::Opaque;

            template <>
            const core::Type WhichType<void>::type = core::Type::Nil;

            template <>
            const core::Type WhichType<int>::type = core::Type::Integer;

            template <>
            const core::Type WhichType<float>::type = core::Type::Floating;

            template <>
            const core::Type WhichType<std::string>::type = core::Type::String;

            template <>
            const core::Type WhichType<bool>::type = core::Type::Boolean;



            core::Some Callback::call(std::vector<core::Some> const& args) const
            {
                return _m_impl->call(args);
            }

            core::Prototype Callback::prototype() const
            {
                return _m_impl->prototype();
            }

            std::string const& Callback::name() const{
                return _m_name;
            }