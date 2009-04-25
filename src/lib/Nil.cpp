#include "Nil.h"
#include "Runtime.h"
#include "SnowString.h"
#include "Function.h"

namespace snow {
	VALUE nil_to_string(VALUE self, uint64_t num_args, VALUE*) {
		return create_string("");
	}

	VALUE nil_inspect(VALUE self, uint64_t, VALUE*) {
		return new String("nil");
	}
	
	Object* nil_prototype() {
		static Object* proto = NULL;
		if (proto) return proto;
		proto = new(kMalloc) Object;
		proto->set_by_string("to_string", new Function(nil_to_string));
		IObject* v_inspect = new Function(nil_inspect);
		proto->set_by_string("inspect", v_inspect);
		proto->set_by_string("name", new String("nil"));
		return proto;
	}
}
