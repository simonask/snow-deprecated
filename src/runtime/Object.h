#ifndef OBJECT_H_ZOHWWN1D
#define OBJECT_H_ZOHWWN1D

#include "base/Basic.h"
#include "Value.h"
#include "ThinObject.h"
#include "CppStackFrame.h"
#include "ValueMap.h"

#include <stdarg.h>
#include <map>
#include <unordered_map>
#include <string>
#include <mutex>

namespace snow {
class Object;

Ptr<Object> object_prototype();

class Object : public ThinObject {
protected:
	GC_ROOTS;
public:
	struct PropertyPair {
		Value getter;
		Value setter;
		PropertyPair() {}
		PropertyPair(const Value& getter, const Value& setter) : getter(getter), setter(setter) {}
		PropertyPair(const PropertyPair& other) : getter(other.getter), setter(other.setter) {}
	};
	typedef RBTree<Value, PropertyPair, ImmediateComparator> Properties;
private:
	ImmediateMap m_Members;
	Properties m_Properties;
	bool m_GCLock;
	
	bool gc_try_lock() { bool v = !m_GCLock; if (v) { m_GCLock = true; } return v; }
	void gc_unlock() { m_GCLock = false; }
	void gc_property_root_node(GarbageAllocator& _gc, GCOperation _op, Properties::Node*& node);
public:
	explicit Object(const Ptr<Object>& prototype = NULL) : ThinObject(prototype) {}
	virtual void initialize(const Ptr<Object>& proto = NULL) {}
	Object(const Object& other) : ThinObject(other), m_Members(other.m_Members) {}
	virtual void initialize(const Object&) {}
	virtual ~Object() {}
	Ptr<IObject> copy() const { return gc_new<Object>(*this); }
	
	const ImmediateMap& members() const { return m_Members; }
	virtual bool has_member(Symbol member) const { return m_Members.find(member); }
	virtual Value set_raw(Symbol member, const Value& value);
	virtual Value get_raw(Symbol member) const;
	virtual Value get(const Value& self, Symbol member) const;
	virtual Value set(const Value& self, Symbol member, const Value& val);

	virtual const PropertyPair* property(Symbol name) const;
	virtual void set_property(Symbol name, const Value& getter, const Value& setter);
	virtual void set_property_getter(Symbol name, const Value& getter);
	virtual void set_property_setter(Symbol name, const Value& setter);
};
}

#endif /* end of include guard: OBJECT_H_ZOHWWN1D */
