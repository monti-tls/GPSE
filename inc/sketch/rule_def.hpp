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

#ifndef __GPSE_SKETCH_RULE_DEF_H__
#define __GPSE_SKETCH_RULE_DEF_H__

#include "lang/lexer.hpp"

#include <iostream>

namespace gpse
{
    namespace sketch
    {
        enum
        {
            LPAR = lang::Token::Good,
            RPAR,
            TIMES,
            SLASH,
            PLUS,
            MINUS,
            AND,
            OR,
            NOT,
            LT,
            LTE,
            GT,
            GTE,
            EQ,
            EQUALS,
            SEMICOLON,
            COMMA,
            LCURLY,
            RCURLY,
            NEQ,
            NUMBER,
            STRING,
            K_TRUE,
            K_FALSE,
            K_RETURN,
            K_FUN,
            K_IF,
            K_ELIF,
            K_ELSE,
            K_WHILE,
            SYMBOL,
            IDENT
        };

        void setupLexer(lang::Lexer& lexer);
    }
}

#endif // __GPSE_SKETCH_RULE_DEF_H__
