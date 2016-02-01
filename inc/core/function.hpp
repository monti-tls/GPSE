#ifndef __GPSE_CORE_FUNCTION_H__
#define __GPSE_CORE_FUNCTION_H__

#include "core/type.hpp"
#include "core/variable.hpp"
#include "core/prototype.hpp"

#include <vector>
#include <string>

namespace gpse
{
    namespace lang
    {
        class Node;
    }

    namespace core
    {
        //! Represents a scripted function, which consists
        //!   of a gpse::core::Prototype (holding the arguments' types and names),
        //!   and of course of the actual code of the function, stored
        //!   as an AST chunk.
        class Function
        {
        public:
            //! Create an empty function.
            Function();

            //! Create a function w/ a name and prototype. One must call
            //!   setNode() later to setup the function's implementation.
            //! \param name The name of the function (only for
            //!             display purposes, the exposed name of the
            //!             function is stored in the gpse::core::Scope object)
            //! \param prototype The actual prototype of the function.
            Function(std::string const& name, Prototype const& prototype);

            ~Function();

            //! Get the name of this function.
            //! \return The name as a const reference.
            std::string const& name() const;

            //! Get the prototype of this function.
            //! \return The prototype as a const reference.
            Prototype const& prototype() const;

            //! Set this function's code as an AST chunk.
            //! \param node The root node of the AST chunk
            //!             (*not* freed upon destruction of this object).
            void setNode(lang::Node* node);

            //! Get this function's code as an AST chunk.
            //! \return A pointer to the root node of the AST.
            lang::Node* node() const;

        private:
            std::string _m_name;
            Prototype _m_prototype;
            lang::Node* _m_node;
        };
    }
}

#endif // __GPSE_CORE_FUNCTION_H__
