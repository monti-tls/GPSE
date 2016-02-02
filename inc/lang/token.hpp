#ifndef __GPSE_LANG_TOKEN_H__
#define __GPSE_LANG_TOKEN_H__

#include "core/some.hpp"

namespace gpse
{
    namespace lang
    {
        class Token
        {
        public:
            enum
            {
                Bad,
                Eof,
                None, // used for comments
                Good  // first good token number
            };

        public:
            Token(int which = 0, core::Some const& value = core::Some());
            ~Token() = default;

            bool operator==(Token const& rhs);
            bool operator!=(Token const& rhs);

            int which;
            core::Some value;

            struct
            {
                int line;
                int col;
                int pos;
            } debug;
        };

        extern const Token eofToken;
        extern const Token badToken;
    }
}

#endif // __GPSE_LEX_TOKEN_H__
