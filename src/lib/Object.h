#ifndef OBJECT_H_ZOHWWN1D
#define OBJECT_H_ZOHWWN1D

#include "Basic.h"
#include "Value.h"
#include <stdarg.h>
#include <map>
#include <unordered_map>
#include <string>
#include "SnowAssert.h"
#include "RefPtr.h"
#include "SuperFastHash.h"
#include "Garbage.h"
#include "Handle.h"

namespace snow {
class Object;

Handle& object_prototype();

class Object : public Garbage {
public:
	typedef std::unordered_map<std::string, Handle, SuperFastHash> Members;
private:
	Handle m_Prototype;
	RefPtr<Members> m_Members;
public:
	explicit Object(Object* prototype = NULL) : m_Prototype(prototype), m_Members(new Members) {}
	Object(const Object& other) : m_Prototype(other.m_Prototype), m_Members(other.m_Members) {}
	virtual ~Object() {}
	VALUE call(VALUE self, uint64_t num_args = 0, ...);
	virtual VALUE va_call(VALUE self, uint64_t num_args, va_list& ap);
	virtual VALUE copy() const { return value(new Object(*this)); }
	
	const Members& members() const { return *m_Members; }
	VALUE set(const char* member, VALUE value);
	VALUE get(const char* member) const;
	const Handle& prototype() const { return m_Prototype ? m_Prototype : object_prototype(); }
	void set_prototype(Handle& proto) { m_Prototype = proto; }
};
}

#endif /* end of include guard: OBJECT_H_ZOHWWN1D */
