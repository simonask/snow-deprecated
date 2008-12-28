#ifndef OBJECT_H_ZOHWWN1D
#define OBJECT_H_ZOHWWN1D

#include "Basic.h"
#include "Value.h"
#include <stdarg.h>
#include <map>
#include <string>
#include "Assert.h"

namespace snot {
class Object;

Object* object_prototype();

class Object {
private:
	const Object* m_Prototype;
	std::map<std::string, VALUE> m_Members;
public:
	explicit Object(const Object* prototype = NULL) : m_Prototype(prototype) {}
	Object(const Object& other) : m_Prototype(other.m_Prototype) {}
	VALUE call(VALUE self, uint64_t num_args = 0, ...);
	virtual VALUE va_call(VALUE self, uint64_t num_args, va_list& ap);
	Object* copy(bool deep = false) const;
	
	std::map<std::string, VALUE>& members() { return m_Members; }
	const std::map<std::string, VALUE>& members() const { return m_Members; }
	VALUE set(const char* member, VALUE value);
	VALUE get(const char* member) const;
	const Object* prototype() const { return m_Prototype ? m_Prototype : object_prototype(); }
};
}

#endif /* end of include guard: OBJECT_H_ZOHWWN1D */
