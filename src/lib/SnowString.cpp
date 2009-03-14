#include "SnowString.h"
#include "Runtime.h"
#include "RuntimeMacros.h"

namespace snow {
	static Handle<Object> StringPrototype = NULL;
	
	static VALUE string_to_string(Scope* scope) {
		ASSERT_OBJECT(SELF, String);
		return SELF;
	}
	
	static VALUE string_equals(Scope* scope) {
		ASSERT_OBJECT(SELF, String);
		ASSERT_ARGS(NUM_ARGS >= 1);
		auto str = SELF.cast<String>();
		if (ARGS[0] == str)
			return value(true);
		else {
			Handle<String> arg = ValueHandle(ARGS[0]).cast<String>();
			return value(str->str() == arg->str());
		}
	}
	
	Handle<Object>& string_prototype() {
		if (StringPrototype)
			return StringPrototype;
	 	StringPrototype = new Object;
		StringPrototype->set("name", create_string("String"));
		StringPrototype->set("to_string", create_function(string_to_string));
		StringPrototype->set("=", create_function(string_equals));
		return StringPrototype;
	}
}