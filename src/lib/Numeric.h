#ifndef NUMERIC_H_VYDCMUQZ
#define NUMERIC_H_VYDCMUQZ

#include "Runtime.h"
#include <cmath>

namespace snow {
	static VALUE numeric_add(VALUE self, uint64_t num_args, VALUE* args) {
		if (num_args == 0)
			return self;
		
		ASSERT(is_numeric(self) && is_numeric(args[0]));
		
		if (is_float(self) || is_float(args[0])) {
			return value(floatnum(call_method(self, "to_f", 0)) + floatnum(call_method(args[0], "to_f", 0)));
		} else {
			return value(integer(self) + integer(args[0]));
		}
	}
	
	static VALUE numeric_sub(VALUE self, uint64_t num_args, VALUE* args) {
		if (num_args == 0)
			return self;
		
		ASSERT(is_numeric(self) && is_numeric(args[0]));
		
		if (is_float(self) || is_float(args[0])) {
			return value(floatnum(call_method(self, "to_f", 0)) - floatnum(call_method(args[0], "to_f", 0)));
		} else {
			return value(integer(self) - integer(args[0]));
		}
	}
	
	static VALUE numeric_mul(VALUE self, uint64_t num_args, VALUE* args) {
		ASSERT_ARGS(num_args > 0)
		ASSERT(is_numeric(self) && is_numeric(args[0]));
		
		if (is_float(self) || is_float(args[0])) {
			return value(floatnum(call_method(self, "to_f", 0)) * floatnum(call_method(args[0], "to_f", 0)));
		} else {
			return value(integer(self) * integer(args[0]));
		}
	}
		
	static VALUE numeric_div(VALUE self, uint64_t num_args, VALUE* args) {
		ASSERT_ARGS(num_args > 0)
		ASSERT(is_numeric(self) && is_numeric(args[0]));
		
		if (is_float(self) || is_float(args[0])) {
			return value(floatnum(call_method(self, "to_f", 0)) / floatnum(call_method(args[0], "to_f", 0)));
		} else {
			return value(integer(self) / integer(args[0]));
		}
	}
}

#endif /* end of include guard: NUMERIC_H_VYDCMUQZ */
