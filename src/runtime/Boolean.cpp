#include "Boolean.h"
#include "Runtime.h"
#include "SnowString.h"
#include "Function.h"

namespace snow {
	Value boolean_to_string(const Value& self, const Arguments&) {
		return gc_new<String>(is_true(self.value()) ? "true" : "false");
	}
	
	Value boolean_not(const Value& self, const Arguments&) {
		return value(!boolean(self.value()));
	}
	
	Ptr<Object> boolean_prototype() {
		static Ptr<Object> proto;
		if (proto) return proto;
		proto = malloc_new<Object>();
		proto->set_raw("to_string", gc_new<Function>(boolean_to_string));
		proto->set_raw("inspect", gc_new<Function>(boolean_to_string));
		proto->set_raw("name", gc_new<Function>(boolean_to_string));
		proto->set_raw("not", gc_new<Function>(boolean_not));
		return proto;
	}
}
