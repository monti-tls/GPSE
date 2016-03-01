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

#ifndef __GPSE_CORE_SYMBOL_H__
#define __GPSE_CORE_SYMBOL_H__

#include "core/object.hpp"
#include <string>

namespace core
{
    class Symbol
    {
    public:
        Symbol(std::string const& name = "?");

        std::string const& name() const;
        void bind(Object* target);
        bool bound() const;
        Object* target() const;

    private:
        std::string m_name;
        Object* m_target;
    };
}

#endif // __GPSE_CORE_SYMBOL_H__
