#include "Boolean.h"
#include "Runtime.h"
#include "SnowString.h"
#include "Function.h"

namespace snow {
	VALUE boolean_to_string(VALUE self, uint64_t num_args, VALUE*) {
		return gc_new<String>(is_true(self) ? "true" : "false");
	}
	
	VALUE boolean_not(VALUE self, uint64_t num_args, VALUE*) {
		return value(!boolean(self));
	}
	
	Object* boolean_prototype() {
		static Object* proto = NULL;
		if (proto) return proto;
		proto = malloc_new<Object>();
		proto->set_raw_s("to_string", gc_new<Function>(boolean_to_string));
		proto->set_raw_s("inspect", gc_new<Function>(boolean_to_string));
		proto->set_raw_s("name", gc_new<Function>(boolean_to_string));
		proto->set_raw_s("not", gc_new<Function>(boolean_not));
		return proto;
	}
}
