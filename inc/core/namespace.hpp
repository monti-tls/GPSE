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

#ifndef __GPSE_CORE_NAMESPACE_H__
#define __GPSE_CORE_NAMESPACE_H__

#include "core/namespace_layer.hpp"
#include "core/symbol.hpp"

#include <string>
#include <map>

namespace core
{
    class Namespace
    {
    public:
        Namespace();
        ~Namespace();

        void set(std::string const& name);
        bool find(std::string const& name, Symbol** found = nullptr);
        bool findLocal(std::string const& name, Symbol** found = nullptr);

        void down();
        void up();

    private:
        NamespaceLayer m_root;
        NamespaceLayer* m_current;
    };
}

#endif // __GPSE_CORE_NAMESPACE_H__
