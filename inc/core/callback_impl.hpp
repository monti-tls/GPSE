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

#ifndef __GPSE_CORE_CALLBACK_IMPL_H__
#define __GPSE_CORE_CALLBACK_IMPL_H__

namespace core
{
    template <typename TRet, typename... TArgs>
    Object CallbackImpl<TRet, TArgs...>::invoke(std::vector<Object> const& args)
    {
        std::tuple<TArgs...> tp = this->M_buildTuple(args.begin(), (TArgs*)nullptr...);

        return ObjectFactory::build(apply(this->m_fun, tp));
    }

    template <typename... TArgs>
    Object CallbackImpl<void, TArgs...>::invoke(std::vector<Object> const& args)
    {
        std::tuple<TArgs...> tp = this->M_buildTuple(args.begin(), (TArgs*)nullptr...);
        apply(this->m_fun, tp);

        return Object::Nil;
    }
}

#endif // __GPSE_CORE_CALLBACK_IMPL_H__
