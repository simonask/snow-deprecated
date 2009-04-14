#include "Float.h"
#include "Function.h"
#include "SnowString.h"
#include <sstream>

namespace snow {
	static VALUE float_add(VALUE self, uint64_t num_args, VALUE* args) {
		ASSERT(is_float(self));
		ASSERT_ARGS(num_args > 0);
		ASSERT(is_float(args[0]));
		float a = floatnum(self);
		float b = floatnum(args[0]);
		return value(a + b);
	}
	
	static VALUE float_sub(VALUE self, uint64_t num_args, VALUE* args) {
		ASSERT(is_float(self));
		ASSERT_ARGS(num_args > 0);
		ASSERT(is_float(args[0]));
		float a = floatnum(self);
		float b = floatnum(args[0]);
		return value(a - b);
	}
	
	static VALUE float_mul(VALUE self, uint64_t num_args, VALUE* args) {
		ASSERT(is_float(self));
		ASSERT_ARGS(num_args > 0);
		ASSERT(is_float(args[0]));
		float a = floatnum(self);
		float b = floatnum(args[0]);
		return value(a * b);
	}
	
	static VALUE float_div(VALUE self, uint64_t num_args, VALUE* args) {
		ASSERT(is_float(self));
		ASSERT_ARGS(num_args > 0);
		ASSERT(is_float(args[0]));
		float a = floatnum(self);
		float b = floatnum(args[0]);
		return value(a / b);
	}
	
	static VALUE float_to_i(VALUE self, uint64_t num_args, VALUE* args) {
		ASSERT(is_float(self));
		int64_t a = (int64_t)floatnum(self);
		return value(a);
	}
	
	static VALUE float_to_string(VALUE self, uint64_t num_args, VALUE* args) {
		ASSERT_ARGS(num_args == 0);
		std::stringstream ss;
		ss << floatnum(self);
		return value(new String(ss.str()));
	}
	
	Handle<Object>& float_prototype() {
		static Permanent<Object> proto;
		if (proto) return proto;
		proto = new Object;
		proto->set_by_string("+", new Function(float_add));
		proto->set_by_string("-", new Function(float_sub));
		proto->set_by_string("*", new Function(float_mul));
		proto->set_by_string("/", new Function(float_div));
		proto->set_by_string("to_i", new Function(float_to_i));
		proto->set_by_string("to_string", new Function(float_to_string));
		proto->set_by_string("inspect", new Function(float_to_string));
		return proto;
	}
}
