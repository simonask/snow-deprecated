#ifndef OBJECT_H_ZOHWWN1D
#define OBJECT_H_ZOHWWN1D

#include "Basic.h"
#include "Value.h"
#include <stdarg.h>
#include <map>
#include <unordered_map>
#include <string>
#include "Assert.h"
#include "RefPtr.h"

namespace snot {
class Object;

Object* object_prototype();

class Object {
public:
	typedef std::unordered_map<std::string, VALUE> Members;
private:
	const Object* m_Prototype;
	RefPtr<Members> m_Members;
public:
	explicit Object(const Object* prototype = NULL) : m_Prototype(prototype), m_Members(new Members) {}
	Object(const Object& other) : m_Prototype(other.m_Prototype), m_Members(other.m_Members) {}
	VALUE call(VALUE self, uint64_t num_args = 0, ...);
	virtual VALUE va_call(VALUE self, uint64_t num_args, va_list& ap);
	
	const Members& members() const { return *m_Members; }
	VALUE set(const char* member, VALUE value);
	VALUE get(const char* member) const;
	const Object* prototype() const { return m_Prototype ? m_Prototype : object_prototype(); }
};
}

#endif /* end of include guard: OBJECT_H_ZOHWWN1D */
