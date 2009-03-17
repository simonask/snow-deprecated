#include "Integer.h"
#include "Runtime.h"
#include "SnowString.h"
#include <sstream>

namespace snow {
	static VALUE integer_puts(VALUE self, uint64_t num_args, VALUE* args) {
		int64_t n = integer(self);
		printf("%lld\n", n);
		return nil();
	}
	
	static VALUE integer_add(VALUE self, uint64_t num_args, VALUE* args) {
		if (num_args == 0)
			return self;
		int64_t a = integer(self);
		ASSERT(is_integer(args[0]));
		int64_t b = integer(args[0]);
		return value(a + b);
	}
	
	static VALUE integer_to_string(VALUE self, uint64_t num_args, VALUE* args) {
		std::stringstream ss;
		ss << integer(self);
		return value(new String(ss.str()));
	}
	
	static VALUE integer_less(VALUE self, uint64_t num_args, VALUE* args) {
		ASSERT_ARGS(num_args >= 1);
		return value(self < args[0]);
	}
	
 	Handle<Object>& integer_prototype() {
		static Handle<Object> ip;
		if (ip) return ip;
		ip = new Object(object_prototype());
		ip->set("name", create_string("Integer"));
		ip->set("puts", new Function(integer_puts));
		ip->set("+", new Function(integer_add));
		ip->set("to_string", new Function(integer_to_string));
		ip->set("<", new Function(integer_less));
		return ip;
	}
}
