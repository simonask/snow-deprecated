#include "Integer.h"
#include "Runtime.h"
#include "SnowString.h"
#include <sstream>
#include <cmath>

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
	
	static VALUE integer_sub(VALUE self, uint64_t num_args, VALUE* args) {
		if (num_args == 0)
			return value(0-integer(self));
		int64_t a = integer(self);
		ASSERT(is_integer(args[0]));
		int64_t b = integer(args[0]);
		return value(a - b);
	}
	
	static VALUE integer_mul(VALUE self, uint64_t num_args, VALUE* args) {
		if (num_args == 0)
			return self;
		int64_t a = integer(self);
		ASSERT(is_integer(args[0]));
		int64_t b = integer(args[0]);
		return value(a * b);
	}
	
	static VALUE integer_div(VALUE self, uint64_t num_args, VALUE* args) {
		if (num_args == 0)
			return self;
		int64_t a = integer(self);
		ASSERT(is_integer(args[0]));
		int64_t b = integer(args[0]);
		return value(a / b);
	}
	
	static VALUE integer_mod(VALUE self, uint64_t num_args, VALUE* args) {
		if (num_args == 0)
			return self;
		int64_t a = integer(self);
		ASSERT(is_integer(args[0]));
		int64_t b = integer(args[0]);
		return value(a % b);
	}

	static VALUE integer_power(VALUE self, uint64_t num_args, VALUE* args) {
		if (num_args == 0)
			return self;
		int64_t a = integer(self);
		ASSERT(is_integer(args[0]));
		int64_t b = integer(args[0]);
		int64_t v = pow(a,b);
		return value(v);
	}

	static VALUE integer_to_string(VALUE self, uint64_t num_args, VALUE* args) {
		std::stringstream ss;
		int base = 10;
		if (num_args > 0) {
			ASSERT(is_integer(args[0]));
			base = integer(args[0]);
		}

		switch (base) {
			case 10:
				ss << integer(self); break;
			case 16:
				ss << "0x" << std::hex << integer(self); break;
			/*case 8:
				ss << std::octal << integer(self); break;*/
			default:
				error("Unsupported base: %d", base);
				TRAP();
		}

		return value(new String(ss.str()));
	}
	
	static VALUE integer_lt(VALUE self, uint64_t num_args, VALUE* args) {
		ASSERT_ARGS(num_args >= 1);
		return value(integer(self) < integer(args[0]));
	}
	
	static VALUE integer_lte(VALUE self, uint64_t num_args, VALUE* args) {
		ASSERT_ARGS(num_args >= 1);
		return value(integer(self) <= integer(args[0]));
	}
	
	static VALUE integer_gt(VALUE self, uint64_t num_args, VALUE* args) {
		ASSERT_ARGS(num_args >= 1);
		return value(integer(self) > integer(args[0]));
	}
	
	static VALUE integer_gte(VALUE self, uint64_t num_args, VALUE* args) {
		ASSERT_ARGS(num_args >= 1);
		return value(integer(self) >= integer(args[0]));
	}
	
	static VALUE integer_sqrt(VALUE self, uint64_t num_args, VALUE* args) {
		ASSERT_ARGS(num_args == 0);
		float v = sqrt(integer(self));
		return value(v);
	}
	
 	Handle<Object>& integer_prototype() {
		static Permanent<Object> ip;
		if (ip) return ip;
		ip = new Object(object_prototype());
		ip->set_by_string("name", create_string("Integer"));
		ip->set_by_string("puts", new Function(integer_puts));
		ip->set_by_string("+", new Function(integer_add));
		ip->set_by_string("-", new Function(integer_sub));
		ip->set_by_string("*", new Function(integer_mul));
		ip->set_by_string("/", new Function(integer_div));
		ip->set_by_string("%", new Function(integer_mod));
		ip->set_by_string("**", new Function(integer_power));
		ip->set_by_string("to_string", new Function(integer_to_string));
		ip->set_by_string("<", new Function(integer_lt));
		ip->set_by_string("<=", new Function(integer_lte));
		ip->set_by_string(">", new Function(integer_gt));
		ip->set_by_string(">=", new Function(integer_gte));
		ip->set_by_string("sqrt", new Function(integer_sqrt));
		return ip;
	}
}
