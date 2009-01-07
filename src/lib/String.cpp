#include "String.h"
#include "Runtime.h"

namespace snot {
	static Object* StringPrototype = NULL;
	
	
	
	Object* string_prototype() {
		if (StringPrototype)
			return StringPrototype;
	 	StringPrototype = new Object;
		StringPrototype->set("name", create_string("String"));
		return StringPrototype;
	}
}