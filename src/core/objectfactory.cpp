#include "core/objectfactory.hpp"

namespace core
{
    std::map<size_t, ObjectFactory::Constructor> ObjectFactory::m_constructors;
}
