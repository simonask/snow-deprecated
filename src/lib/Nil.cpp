#include "Nil.h"
#include "Runtime.h"

namespace snow {
	Handle<Object>& nil_prototype() {
		static Handle<Object> proto;
		if (proto) return proto;
		proto = new Object;
		proto->set("to_string", create_string(""));
		proto->set("name", create_string("nil"));
		return proto;
	}
}