#include "Float.h"
#include "Function.h"

namespace snow {
	static Handle<Object> FloatPrototype = NULL;
	
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
	
	Handle<Object>& float_prototype() {
		if (FloatPrototype)
			return FloatPrototype;
		FloatPrototype = new Object;
		FloatPrototype->set("+", new Function(float_add));
		FloatPrototype->set("-", new Function(float_sub));
		FloatPrototype->set("*", new Function(float_mul));
		FloatPrototype->set("/", new Function(float_div));
		FloatPrototype->set("to_i", new Function(float_to_i));
		return FloatPrototype;
	}
}
