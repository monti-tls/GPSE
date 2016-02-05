#include "unstable/object.hpp"
#include "unstable/callable.hpp"

using namespace unstable;

Object Object::Nil = Object();

Object::Object(Object::Kind kind, Some meta)
	: m_kind(kind)
    , m_meta(meta)
{}

Object::~Object()
{}

Object::Kind Object::kind() const
{ return m_kind; }

Some const& Object::meta() const
{ return m_meta; }

bool Object::has(std::string const& id) const
{ return m_members.find(id) != m_members.end(); }

Object& Object::member(std::string const& id)
{ return m_members[id]; }

Object const& Object::member(std::string const& id) const
{
    std::map<std::string, Object>::const_iterator it = m_members.find(id);
    if (it != m_members.end())
        return it->second;

    return Nil;
}

Object Object::invoke(std::vector<Object> const& args) const
{
    if (m_kind == Kind::Callable)
	    return m_meta.as<Callable>().invoke(args);
	else if (has("__call__"))
		return member("__call__").invoke(args);

	throw std::runtime_error("attempt to invoke a non-callable object");
}

Object Object::operator()(std::vector<Object> const& args) const
{ return member("__call__").invoke(args); }

Object Object::operator+(Object const& other) const
{ return member("__add__").invoke({*this, other}); }

Object Object::operator-(Object const& other) const
{ return member("__sub__").invoke({*this, other}); }

Object Object::operator*(Object const& other) const
{ return member("__mul__").invoke({*this, other}); }

Object Object::operator/(Object const& other) const
{ return member("__div__").invoke({*this, other}); }

Object Object::operator%(Object const& other) const
{ return member("__mod__").invoke({*this, other}); }
