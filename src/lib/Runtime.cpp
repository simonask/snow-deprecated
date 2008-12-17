#include "Runtime.h"
#include "Object.h"
#include <stdarg.h>

namespace snot {
	VALUE create_object(Object* prototype) {
		return value(new Object(prototype));
	}
	
	VALUE call(VALUE obj) {
		if (is_object(obj))
			return object(obj)->call();
		else
			return obj;
	}
	
	VALUE send(VALUE obj, const char* message, uint64_t num_args, ...) {
		VALUE ret;
		va_list ap;
		va_start(ap, num_args);
		if (is_object(obj))
			ret = object(obj)->va_send(message, num_args, ap);
		else {
			// TODO: Handle special types
		}
		va_end(ap);
	}
}