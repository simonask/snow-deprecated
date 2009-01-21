#include "Nil.h"
#include "Runtime.h"

namespace snow {
	static Object* NilPrototype = NULL;
	
	Object* nil_prototype() {
		if (NilPrototype)
			return NilPrototype;
		NilPrototype = new Object;
		NilPrototype->set("to_string", create_string("nil"));
		NilPrototype->set("name", create_string("nil"));
		return NilPrototype;
	}
}