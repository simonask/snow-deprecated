#ifndef OBJECT_H_ZOHWWN1D
#define OBJECT_H_ZOHWWN1D

#include "Basic.h"
#include "Value.h"
#include "ThinObject.h"

#include <stdarg.h>
#include <map>
#include <unordered_map>
#include <string>
#include <mutex>

namespace snow {
class Object;

Object* object_prototype();

class Object : public ThinObject {
protected:
	GC_ROOTS;
public:
	// TODO: Use a more GC-friendly hashmap
	typedef std::unordered_map<VALUE, VALUE> Members;

	struct PropertyPair {
		VALUE getter;
		VALUE setter;
		PropertyPair() : getter(NULL), setter(NULL) {}
		PropertyPair(VALUE getter, VALUE setter) : getter(getter), setter(setter) {}
		PropertyPair(const PropertyPair& other) : getter(other.getter), setter(other.setter) {}
	};
	typedef std::unordered_map<VALUE, PropertyPair> Properties;
private:
	Members m_Members; 
	Properties m_Properties;
	std::mutex m_GCMutex;
public:
	explicit Object(const Handle<Object>& prototype = NULL) : ThinObject(prototype) {}
	Object(const Object& other) : ThinObject(other), m_Members(other.m_Members) {}
	virtual ~Object() {}
	IObject* copy() const { return new Object(*this); }
	
	VALUE va_call(VALUE self, uint64_t num_args, va_list&);
	const Members& members() const { return m_Members; }
	virtual bool has_member(VALUE member) const;
	virtual VALUE set(VALUE member, VALUE value);
	virtual VALUE get(VALUE member) const;
	virtual VALUE get_with_property(VALUE self, VALUE member) const;
	virtual VALUE set_with_property(VALUE self, VALUE member, VALUE val);

	virtual const PropertyPair* property(VALUE name) const;
	virtual void set_property(VALUE name, VALUE getter, VALUE setter);
	virtual void set_property_getter(VALUE name, VALUE getter);
	virtual void set_property_setter(VALUE name, VALUE setter);
};
}

#endif /* end of include guard: OBJECT_H_ZOHWWN1D */
