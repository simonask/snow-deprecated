#include "Nil.h"
#include "Runtime.h"

namespace snow {
	VALUE nil_to_string(VALUE self, uint64_t num_args, VALUE*) {
		return create_string("");
	}
	
	Handle<Object>& nil_prototype() {
		static Handle<Object> proto;
		if (proto) return proto;
		proto = new Object;
		proto->set("to_string", new Function(nil_to_string));
		proto->set("name", create_string("nil"));
		return proto;
	}
}