#include "Runtime.h"
#include "Object.h"
#include "Integer.h"
#include "Nil.h"
#include "SnowString.h"
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
	
	VALUE create_float(double d) {
		return nil();
	}
	
	VALUE call(VALUE self, uint64_t num_args, ...) {
		if (is_object(self)) {
			Object* obj = object_for(self);
			VALUE ret;
			va_list ap;
			va_start(ap, num_args);
			ret = obj->va_call(self, num_args, ap);
			va_end(ap);
			return ret;
		} else
			return self;
	}
	
	VALUE send(VALUE obj, const char* message) {
		Object* o = object_for(obj);
		return o->get(message);
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
			
			VALUE finalize = send(obj, "finalize");
			call(finalize, 0);
			
			for (auto iter = iterate(obj->members()); iter; ++iter) {
				destroy(iter->second);
			}
			
			delete obj;
		}
	}
	
	const char* value_to_string(VALUE _obj) {
		Object* obj = object_for(_obj);
		
		VALUE to_string = send(obj, "to_string");
		VALUE returned = call(to_string, 0);
		
		assert_object(returned, String);
		
		auto string = object_cast<String>(returned);
		
		return (const char*)(*string);
	}
	
	Object* object_for(VALUE obj) {
		switch (value_type(obj)) {
			case kObjectType:
				// Safeguard: NULL pointers become 'nil'
				return obj ? object(obj) : nil_prototype();
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
					case kNil:
						return nil_prototype();
				}
				break;
		}

		return nil_prototype();
	}
}