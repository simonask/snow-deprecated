#include "Boolean.h"
#include "Runtime.h"
#include "SnowString.h"

namespace snow {
	VALUE boolean_to_string(VALUE self, uint64_t num_args, VALUE*) {
		return create_string(is_true(self) ? "true" : "false");
	}
	
	VALUE boolean_not(VALUE self, uint64_t num_args, VALUE*) {
		return value(!boolean(self));
	}
	
	Handle<Object>& boolean_prototype() {
		static Permanent<Object> proto;
		if (proto) return proto;
		proto = new Object;
		proto->set_by_string("to_string", new Function(boolean_to_string));
		proto->set_by_string("inspect", new Function(boolean_to_string));
		proto->set_by_string("name", new Function(boolean_to_string));
		proto->set_by_string("not", new Function(boolean_not));
		return proto;
	}
}
