#include "Nil.h"
#include "Runtime.h"
#include "SnowString.h"
#include "Exception.h"
#include "Function.h"

namespace snow {
	Value nil_to_string(const Value& self, const Arguments&) {
		return gc_new<String>("");
	}

	Value nil_inspect(const Value& self, const Arguments&) {
		return gc_new<String>("nil");
	}

	Value nil_call(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		throw_exception(gc_new<String>("nil called. This is probably a bug."));
		return nil();
	}
	
	Ptr<Object> nil_prototype() {
		static Ptr<Object> proto;
		if (proto) return proto;
		proto = malloc_new<Object>();
		proto->set_raw("to_string", gc_new<Function>(nil_to_string));
		Ptr<IObject> v_inspect = gc_new<Function>(nil_inspect);
		proto->set_raw("inspect", v_inspect);
		proto->set_raw("name", gc_new<String>("nil"));
		proto->set_raw("__call__", gc_new<Function>(nil_call));
		return proto;
	}
}
