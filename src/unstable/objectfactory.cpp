#include "unstable/objectfactory.hpp"

namespace unstable
{
    std::map<size_t, ObjectFactory::Constructor> ObjectFactory::m_constructors;
}
