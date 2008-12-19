#include "Runtime.h"
#include "Object.h"
#include "Integer.h"
#include "Undefined.h"
#include <stdarg.h>

namespace snot {
	VALUE create_object(Object* prototype) {
		return value(new Object(prototype));
	}
	
	VALUE create_function(FunctionPtr ptr) {
		return value(new Function(ptr));
	}
	
	VALUE call(VALUE obj, VALUE self, uint64_t num_args, ...) {
		if (is_object(obj)) {
			VALUE ret;
			va_list ap;
			va_start(ap, num_args);
			ret = object(obj)->va_call(self, num_args, ap);
			va_end(ap);
			return ret;
		} else
			return obj;
	}
	
	VALUE send(VALUE obj, const char* message, uint64_t num_args, ...) {
		Object* o = NULL;
		switch (value_type(obj)) {
			case kObjectType:
				// Safeguard: NULL pointers become 'undefined'
				o = obj ? object(obj) : undefined_prototype();
				break;
			case kEvenIntegerType:
			case kOddIntegerType:
				o = integer_prototype();
				break;
			default:
			case kSpecialType:
				switch (special_value(obj)) {
					case kTrue:
					case kFalse:
//						o = boolean_prototype();
						break;
					default:
					case kUndefined:
						o = undefined_prototype();
						break;
				}
				break;
		}
		
		VALUE callee = o->get(message);
		VALUE ret;
		if (is_object(callee)) {
			va_list ap;
			va_start(ap, num_args);
			ret = object(callee)->va_call(obj, num_args, ap);
			va_end(ap);
		} else
			ret = callee;
			
		return ret;
	}
}