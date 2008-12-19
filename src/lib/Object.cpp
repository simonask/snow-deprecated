#include "Object.h"
#include "Function.h"

namespace snot {
	VALUE Object::call(VALUE self, uint64_t num_args, ...) {
		va_list ap;
		va_start(ap, num_args);
		VALUE ret = va_call(self, num_args, ap);
		va_end(ap);
		return ret;
	}
	
	VALUE Object::va_call(VALUE self, uint64_t num_args, va_list& ap) {
		return value(this);
	}
	
	VALUE Object::set(const char* member, VALUE value) {
		m_Members[member] = value;
	}
	
	VALUE Object::get(const char* member) {
		auto iter = m_Members.find(member);
		if (iter != m_Members.end())
			return iter->second;
		else
			return undefined();
	}
}