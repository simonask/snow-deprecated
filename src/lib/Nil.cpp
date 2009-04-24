#include "Nil.h"
#include "Runtime.h"
#include "SnowString.h"

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
		proto->__gc_debug(true);
		debug("Nil prototype at 0x%llx", proto);
		proto->set_by_string("to_string", new Function(nil_to_string));
		IObject* v_inspect = new Function(nil_inspect);
		v_inspect->__gc_debug(true);
		proto->set_by_string("inspect", v_inspect);
		proto->set_by_string("name", new String("nil"));
		return proto;
	}
}
