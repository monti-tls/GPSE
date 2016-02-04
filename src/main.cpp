#include "unstable/object.hpp"
#include "unstable/callable.hpp"
#include "unstable/callback.hpp"
#include "unstable/objectfactory.hpp"

// be careful to include this at the very end
#include "unstable/callback_impl.hpp"

#include <string>
#include <iostream>
#include <functional>

using namespace unstable;

// A class <=> a constructor, setting up the object with the appropriate methods
// methods <=> a function, taking at least the 'this' argument
// object : can be callable or scalar

Object Object_constructor(Object const& value)
{ return value; }

int int_add(int a, int b)
{ return a + b; }

int int_sub(int a, int b)
{ return a - b; }

Object int_constructor(int const& value)
{
    Object obj(Object::Kind::Scalar, Some(value));

    obj.member("__add__") = ObjectFactory::build(&int_add);
    obj.member("__sub__") = ObjectFactory::build(&int_sub);

    return obj;
}

void yolo()
{ std::cout << "yolo !" << std::endl; }

int main()
{
    Object obj(Object::Kind::Scalar);
    obj.member("__call__") = ObjectFactory::build(&yolo);
    obj.invoke({});

    ObjectFactory::registerType<Object>(&Object_constructor);
    ObjectFactory::registerType<int>(&int_constructor);

    Object a = ObjectFactory::build(123);
    Object b = ObjectFactory::build(321);

    std::cout << a.member("__add__").invoke({a, b}).unwrap<int>() << std::endl;

    return 0;
}
