#include "Runtime.h"
#include "Object.h"
#include "Integer.h"
#include "Undefined.h"
#include "String.h"
#include <stdarg.h>

namespace snow {
	VALUE create_object(Object* prototype) {
		return value(new Object(prototype));
	}
	
	VALUE create_function(FunctionPtr ptr) {
		return value(new Function(ptr));
	}
	
	VALUE create_string(const char* str) {
		return value(new String(str));
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
		Object* o = object_for(obj);
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
	
	VALUE copy(VALUE obj) {
		if (is_object(obj))
			return object(obj)->copy();
		else
			return obj;
	}
	
	void destroy(VALUE _obj) {
		if (is_object(_obj)) {
			Object* obj = object(_obj);
			
			send(obj, "finalize", 0);
			
			for (auto iter = iterate(obj->members()); iter; ++iter) {
				destroy(iter->second);
			}
			
			delete obj;
		}
	}
	
	Object* object_for(VALUE obj) {
		Object* receiver = NULL;
		switch (value_type(obj)) {
			case kObjectType:
				// Safeguard: NULL pointers become 'undefined'
				return obj ? object(obj) : undefined_prototype();
			case kEvenIntegerType:
			case kOddIntegerType:
				return integer_prototype();
			default:
			case kSpecialType:
				switch (special_value(obj)) {
					case kTrue:
					case kFalse:
//						return boolean_prototype();
						break;
					default:
					case kUndefined:
						return undefined_prototype();
				}
				break;
		}

		return undefined_prototype();
	}
}