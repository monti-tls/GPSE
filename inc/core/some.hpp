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

namespace core
{
    class Some
    {
    public:
        Some()
            : m_data{nullptr}
        {
        }

        template <typename T>
        Some(T const& t)
            : m_data{new Data<T>(t)}
        {
        }

        Some(Some const& cpy)
        {
            m_data = cpy.m_data ? cpy.m_data->copy() : nullptr;
        }

        Some(Some&& tmp)
        {
            m_data = tmp.m_data;
            tmp.m_data = nullptr;
        }

        ~Some()
        {
            if(m_data)
                delete m_data;
        }

        Some& operator=(Some const& cpy)
        {
            if(m_data)
                delete m_data;
            m_data = cpy.m_data ? cpy.m_data->copy() : nullptr;
            return *this;
        }

        Some& operator=(Some&& tmp)
        {
            if(m_data)
                delete m_data;
            m_data = tmp.m_data;
            tmp.m_data = nullptr;
            return *this;
        }

        void clear()
        {
            if(!m_data)
                return;

            delete m_data;
            m_data = nullptr;
        }

        bool empty() const
        {
            return !m_data;
        }

        bool valid() const
        {
            return m_data;
        }

        template <typename T>
        bool is() const
        {
            return m_data ? m_data->is(m_typeId<T>()) : false;
        }

        bool same(Some const& other) const
        {
            if(!other.m_data && !m_data)
                return true;
            if(!other.m_data || !m_data)
                return false;

            return other.m_data->id() == m_data->id();
        }

        template <typename T>
        T& as()
        {
            return dynamic_cast<Data<T>&>(*m_data).get();
        }

        template <typename T>
        T const& as() const
        {
            return dynamic_cast<Data<T>&>(*m_data).get();
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
        static size_t m_typeId()
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
                return id == m_typeId<T>();
            }

            size_t id() const
            {
                return m_typeId<T>();
            }

            Base* copy() const
            {
                return new Data{get()};
            }
        };

    private:
        Base* m_data;
    };
}

#endif // __GPSE_CORE_SOME_H__
