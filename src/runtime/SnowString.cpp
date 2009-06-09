#include <sstream>
#include "SnowString.h"
#include "Runtime.h"
#include "Function.h"

namespace snow {
	static VALUE string_to_string(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		ASSERT_OBJECT(self, String);
		return self;
	}

	static VALUE string_inspect(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		String* str = object_cast<String>(self);
		ASSERT(str);
		// TODO: Escape properly
		std::stringstream ss;
		ss << "\"";
		ss << str->str();
		ss << "\"";
		return gc_new<String>(ss.str());
	}
	
	static VALUE string_equals(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		ASSERT_OBJECT(self, String);
		ASSERT_ARGS(num_args >= 1);
		auto str = object_cast<String>(self);
		if (args[0] == str)
			return value(true);
		else {
			Handle<String> other = object_cast<String>(args[0]);
			return value(other && str->str() == other->str());
		}
	}
	
	static VALUE string_plus(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		ASSERT_OBJECT(self, String);
		ASSERT_ARGS(num_args == 1);
		
		Handle<String> str = object_cast<String>(self);
		
		std::stringstream ss;
		ss << str->str() << value_to_string(args[0]);
		VALUE v = gc_new<String>(ss.str().c_str());
		return v;
	}
	
	static VALUE string_reverse(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		ASSERT_OBJECT(self, String);
		ASSERT_ARGS(num_args == 0);
		
		std::stringstream ss;
		std::string str = object_cast<String>(self)->str();
		for (auto iter = str.rbegin(); iter != str.rend(); iter++)
			ss << *iter;
		
		VALUE v = gc_new<String>(ss.str().c_str());
		return v;
	}
	
	Object* string_prototype() {
		static Object* proto = NULL;
		if (proto) return proto;
	 	proto = malloc_new<Object>();
		return proto;
	}

	void string_prototype_init() {
		Object* proto = string_prototype();
		proto->set_raw_s("name", gc_new<String>("String"));
		proto->set_raw_s("to_string", gc_new<Function>(string_to_string));
		proto->set_raw_s("inspect", gc_new<Function>(string_inspect));
		proto->set_raw_s("=", gc_new<Function>(string_equals));
		proto->set_raw_s("+", gc_new<Function>(string_plus));
		proto->set_raw_s("<<", gc_new<Function>(string_plus));
		proto->set_raw_s("reverse", gc_new<Function>(string_reverse));
	}
}
