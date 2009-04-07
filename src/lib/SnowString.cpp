#include <sstream>
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
	
	static VALUE string_plus(VALUE self, uint64_t num_args, VALUE* args) {
		ASSERT_OBJECT(self, String);
		ASSERT_ARGS(num_args == 1);
		
		Handle<String> str = object_cast<String>(self);
		
		std::stringstream ss;
		ss << str->str() << value_to_string(args[0]);
		VALUE v = create_string(ss.str().c_str());
		return v;
	}
	
	static VALUE string_reverse(VALUE self, uint64_t num_args, VALUE* args) {
		ASSERT_OBJECT(self, String);
		ASSERT_ARGS(num_args == 0);
		
		std::stringstream ss;
		std::string str = object_cast<String>(self)->str();
		for (auto iter = str.rbegin(); iter != str.rend(); iter++)
			ss << *iter;
		
		VALUE v = create_string(ss.str().c_str());
		return v;
	}
	
	Handle<Object>& string_prototype() {
		static Permanent<Object> proto;
		if (proto) return proto;
	 	proto = new Object;
		proto->set_by_string("name", create_string("String"));
		proto->set_by_string("to_string", new Function(string_to_string));
		proto->set_by_string("=", new Function(string_equals));
		proto->set_by_string("+", new Function(string_plus));
		proto->set_by_string("<<", new Function(string_plus));
		proto->set_by_string("reverse", new Function(string_reverse));
		return proto;
	}
}
