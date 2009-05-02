#ifndef IOBJECT_H_1VTRMSXU
#define IOBJECT_H_1VTRMSXU

#include "Basic.h"
#include "IGarbage.h"
#include <stdarg.h>

namespace snow {
	class Object; 
	
	class IObject : public IGarbage {
	public:
		virtual Object* prototype() const = 0;
		virtual IObject* copy() const = 0;
		
		virtual uint64_t id() const = 0;
		virtual bool has_member(VALUE name) const = 0;
		virtual VALUE get(VALUE name) const = 0;
		virtual VALUE set(VALUE name, VALUE) = 0;

		virtual VALUE get_with_property(VALUE self, VALUE name) const { return get(name); }
		virtual VALUE set_with_property(VALUE self, VALUE name, VALUE val) { return set(name, val); }
		
		virtual VALUE va_call(VALUE self, uint64_t num_args, va_list&) = 0;
	};
}

#endif /* end of include guard: IOBJECT_H_1VTRMSXU */
