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
		
		virtual bool has_member(const std::string& name) const = 0;
		virtual VALUE get(const std::string& name) const = 0;
		virtual VALUE set(const std::string& name, VALUE) = 0;
		
		virtual VALUE va_call(VALUE self, uint64_t num_args, va_list&) = 0;
	};
}

#endif /* end of include guard: IOBJECT_H_1VTRMSXU */
