#include "Integer.h"
#include "Runtime.h"
#include "SnowString.h"
#include <sstream>

namespace snow {
	static Handle IntegerPrototype = NULL;
	
	static VALUE integer_puts(VALUE self, uint64_t num_args, VALUE*) {
		int64_t n = integer(self);
		printf("%lld\n", n);
		return nil();
	}
	
	static VALUE integer_add(VALUE self, uint64_t num_args, VALUE* args) {
		if (num_args == 0)
			return self;
		int64_t a = integer(self);
		int64_t b = integer(args[0]);
		return value(a + b);
	}
	
	static VALUE integer_to_string(VALUE self, uint64_t num_args, VALUE* args) {
		std::stringstream ss;
		ss << integer(self);
		return value(new String(ss.str()));
	}
	
	static VALUE integer_less(VALUE self, uint64_t num_args, VALUE* args) {
		assert_args(num_args >= 1);
		return value(self < args[0]);
	}
	
	Object* integer_prototype() {
		if (IntegerPrototype)
			return IntegerPrototype;
		Object* ip = new Object(object_prototype());
		ip->set("name", create_string("Integer"));
		ip->set("puts", create_function(integer_puts));
		ip->set("+", create_function(integer_add));
		ip->set("to_string", create_function(integer_to_string));
		ip->set("<", create_function(integer_less));
		IntegerPrototype = ip;
		return IntegerPrototype;
	}
}
