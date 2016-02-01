/*  This file is part of gpse.
 *
 * gpse is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * gpse is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with gpse.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __GPSE_CORE_SOME_H__
#define __GPSE_CORE_SOME_H__

#include <type_traits>
#include <tuple>

namespace gpse
{
    namespace core
    {
        class Some
        {
        public:
            Some()
                : _m_data{nullptr}
            {
            }

            template <typename T>
            Some(T const& t)
                : _m_data{new Data<T>(t)}
            {
            }

            Some(Some const& cpy)
            {
                _m_data = cpy._m_data ? cpy._m_data->copy() : nullptr;
            }

            Some(Some&& tmp)
            {
                _m_data = tmp._m_data;
                tmp._m_data = nullptr;
            }

            ~Some()
            {
                if(_m_data) delete _m_data;
            }

            Some& operator=(Some const& cpy)
            {
                if(_m_data) delete _m_data;
                _m_data = cpy._m_data ? cpy._m_data->copy() : nullptr;
                return *this;
            }

            Some& operator=(Some&& tmp)
            {
                if(_m_data) delete _m_data;
                _m_data = tmp._m_data;
                tmp._m_data = nullptr;
                return *this;
            }

            void clear()
            {
                if(!_m_data) return;

                delete _m_data;
                _m_data = nullptr;
            }

            bool empty() const
            {
                return !_m_data;
            }

            bool valid() const
            {
                return _m_data;
            }

            template <typename T>
            bool is() const
            {
                return _m_data ? _m_data->is(_M_typeId<T>()) : false;
            }

            bool same(Some const& other) const
            {
                if(!other._m_data && !_m_data) return true;
                if(!other._m_data || !_m_data) return false;

                return other._m_data->id() == _m_data->id();
            }

            template <typename T>
            T& as()
            {
                return dynamic_cast<Data<T>&>(*_m_data).get();
            }

            template <typename T>
            T const& as() const
            {
                return dynamic_cast<Data<T>&>(*_m_data).get();
            }

        private:
            template <typename T>
            class Type
            {
            public:
                static void id()
                {
                }
            };

            template <typename T>
            static size_t _M_typeId()
            {
                return reinterpret_cast<size_t>(&Type<T>::id);
            }

            class Base
            {
            public:
                virtual ~Base()
                {
                }

                virtual bool is(size_t) const = 0;
                virtual size_t id() const = 0;
                virtual Base* copy() const = 0;
            };

            template <typename T>
            class Data : public Base, std::tuple<T>
            {
            public:
                using std::tuple<T>::tuple;

                T& get()
                {
                    return std::get<0>(*this);
                }

                T const& get() const
                {
                    return std::get<0>(*this);
                }

                bool is(size_t id) const
                {
                    return id == _M_typeId<T>();
                }

                size_t id() const
                {
                    return _M_typeId<T>();
                }

                Base* copy() const
                {
                    return new Data{get()};
                }
            };

        private:
            Base* _m_data;
        };
    }
}

#endif // __GPSE_CORE_SOME_H__
