#include "SnowString.h"
#include "Runtime.h"
#include "InternalMacros.h"

namespace snow {
	static Handle<Object> StringPrototype = NULL;
	
	static VALUE string_to_string(Scope* scope) {
		ASSERT_OBJECT(SELF, String);
		return SELF;
	}
	
	Handle<Object>& string_prototype() {
		if (StringPrototype)
			return StringPrototype;
	 	StringPrototype = new Object;
		StringPrototype->set("name", create_string("String"));
		StringPrototype->set("to_string", create_function(string_to_string));
		return StringPrototype;
	}
}