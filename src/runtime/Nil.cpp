#include "Nil.h"
#include "Runtime.h"
#include "SnowString.h"
#include "Exception.h"
#include "Function.h"

namespace snow {
	VALUE nil_to_string(VALUE self, uint64_t num_args, VALUE*) {
		return gc_new<String>("");
	}

	VALUE nil_inspect(VALUE self, uint64_t, VALUE*) {
		return gc_new<String>("nil");
	}

	VALUE nil_call(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		throw_exception(gc_new<String>("nil called. This is probably a bug."));
		return nil();
	}
	
	Object* nil_prototype() {
		static Object* proto = NULL;
		if (proto) return proto;
		proto = malloc_new<Object>();
		proto->set_raw_s("to_string", gc_new<Function>(nil_to_string));
		IObject* v_inspect = gc_new<Function>(nil_inspect);
		proto->set_raw_s("inspect", v_inspect);
		proto->set_raw_s("name", gc_new<String>("nil"));
		proto->set_raw_s("__call__", gc_new<Function>(nil_call));
		return proto;
	}
}
