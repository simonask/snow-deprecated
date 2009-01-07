#include "String.h"

namespace snot {
	static Object* StringPrototype = NULL;
	
	
	
	Object* string_prototype() {
		if (StringPrototype)
			return StringPrototype;
	 	StringPrototype = new Object;
		StringPrototype->set("name", value(new String("String")));
		return StringPrototype;
	}
}