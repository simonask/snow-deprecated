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
	
	Handle<Object>& nil_prototype() {
		static Permanent<Object> proto;
		if (proto) return proto;
		proto = new Object;
		proto->set_by_string("to_string", new Function(nil_to_string));
		proto->set_by_string("inspect", new Function(nil_inspect));
		proto->set_by_string("name", create_string("nil"));
		return proto;
	}
}
