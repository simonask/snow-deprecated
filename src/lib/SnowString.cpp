#include "SnowString.h"
#include "Runtime.h"

namespace snow {
	static Handle<Object> StringPrototype = NULL;
	
	static VALUE string_to_string(VALUE self, uint64_t num_args, VALUE* args) {
		ASSERT_OBJECT(self, String);
		return self;
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