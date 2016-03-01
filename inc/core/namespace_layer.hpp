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

#ifndef __GPSE_CORE_NAMESPACE_LAYER_H__
#define __GPSE_CORE_NAMESPACE_LAYER_H__

#include "core/symbol.hpp"

#include <string>
#include <map>
#include <vector>

namespace core
{
    class NamespaceLayer
    {
    public:
        NamespaceLayer(NamespaceLayer* parent = nullptr);
        ~NamespaceLayer();

        void set(std::string const& name);
		bool find(std::string const& name, Symbol** found = nullptr);
        bool findLocal(std::string const& name, Symbol** found = nullptr);

        NamespaceLayer* addChild();
        NamespaceLayer* parent() const;

    private:
    	NamespaceLayer* m_parent;
    	std::map<std::string, Symbol> m_symbols;
    	std::vector<NamespaceLayer> m_children;
    };
}

#endif // __GPSE_CORE_NAMESPACE_LAYER_H__
