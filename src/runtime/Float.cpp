#include "Float.h"
#include "Numeric.h"
#include "Function.h"
#include "SnowString.h"
#include <sstream>

namespace snow {	
	static VALUE float_to_i(VALUE self, uint64_t num_args, VALUE* args) {
		ASSERT(is_float(self));
		int64_t a = (int64_t)floatnum(self);
		return value(a);
	}
	
	static VALUE float_to_f(VALUE self, uint64_t num_args, VALUE* args) {
		ASSERT(is_float(self));
		return self;
	}
	
	static VALUE float_to_string(VALUE self, uint64_t num_args, VALUE* args) {
		ASSERT_ARGS(num_args == 0);
		std::stringstream ss;
		ss << floatnum(self);
		return value(new String(ss.str()));
	}
	
	Object* float_prototype() {
		static Object* proto = NULL;
		if (proto) return proto;
		proto = new(kMalloc) Object;
		proto->set_raw_s("+", new Function(numeric_add));
		proto->set_raw_s("-", new Function(numeric_sub));
		proto->set_raw_s("*", new Function(numeric_mul));
		proto->set_raw_s("/", new Function(numeric_div));
		proto->set_raw_s("to_i", new Function(float_to_i));
		proto->set_raw_s("to_f", new Function(float_to_f));
		proto->set_raw_s("to_string", new Function(float_to_string));
		proto->set_raw_s("inspect", new Function(float_to_string));
		return proto;
	}
}
