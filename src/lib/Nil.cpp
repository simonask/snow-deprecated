#include "Nil.h"
#include "Runtime.h"

namespace snow {
	static Handle<Object> NilPrototype = NULL;
	
	Handle<Object>& nil_prototype() {
		if (NilPrototype)
			return NilPrototype;
		NilPrototype = new Object;
		NilPrototype->set("to_string", create_string(""));
		NilPrototype->set("name", create_string("nil"));
		return NilPrototype;
	}
}