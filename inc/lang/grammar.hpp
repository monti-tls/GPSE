#ifndef __GPSE_LANG_GRAMMAR_H__
#define __GPSE_LANG_GRAMMAR_H__

#include "lang/node.hpp"

#include <functional>

namespace gpse
{
    namespace lang
    {
        class Parser;

        class Grammar
        {
        public:
            typedef std::function<bool(Parser*)> pred_t;
            typedef std::function<Node*(Parser*)> get_t;

        public:
            Grammar(pred_t predicate = [](Parser*)
                    {
                        return false;
                    },
                    get_t get = [](Parser*)
                    {
                        return nullptr;
                    });
            ~Grammar();

            bool predicate(Parser* parser);
            Node* get(Parser* parser);

        private:
            pred_t _m_predicate;
            get_t _m_get;
        };
    }
}

#endif // __GPSE_LANG_GRAMMAR_H__
