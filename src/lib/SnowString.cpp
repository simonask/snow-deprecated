#include "SnowString.h"
#include "Runtime.h"

namespace snow {
	static VALUE string_to_string(VALUE self, uint64_t num_args, VALUE* args) {
		ASSERT_OBJECT(self, String);
		return self;
	}
	
	static VALUE string_equals(VALUE self, uint64_t num_args, VALUE* args) {
		ASSERT_OBJECT(self, String);
		ASSERT_ARGS(num_args >= 1);
		auto str = object_cast<String>(self);
		if (args[0] == str)
			return value(true);
		else {
			Handle<String> arg = object_cast<String>(args[0]);
			return value(str->str() == arg->str());
		}
	}
	
	Handle<Object>& string_prototype() {
		static Handle<Object> proto;
		if (proto) return proto;
	 	proto = new Object;
		proto->set("name", create_string("String"));
		proto->set("to_string", new Function(string_to_string));
		proto->set("=", new Function(string_equals));
		return proto;
	}
}