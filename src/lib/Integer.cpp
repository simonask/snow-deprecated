#include "Integer.h"
#include "Runtime.h"
#include "SnowString.h"
#include "Function.h"
#include "Exception.h"
#include <sstream>
#include <cmath>

#define FUNC new(kMalloc) Function

namespace snow {
	static VALUE integer_puts(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		int64_t n = integer(self);
		printf("%lld\n", n);
		return nil();
	}
	
	static VALUE integer_add(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		if (num_args == 0)
			return self;
		int64_t a = integer(self);
		ASSERT(is_integer(args[0]));
		int64_t b = integer(args[0]);
		return value(a + b);
	}
	
	static VALUE integer_sub(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		if (num_args == 0)
			return value(0-integer(self));
		int64_t a = integer(self);
		ASSERT(is_integer(args[0]));
		int64_t b = integer(args[0]);
		return value(a - b);
	}
	
	static VALUE integer_mul(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		if (num_args == 0)
			return self;
		int64_t a = integer(self);
		ASSERT(is_integer(args[0]));
		int64_t b = integer(args[0]);
		return value(a * b);
	}
	
	static VALUE integer_div(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		if (num_args == 0)
			return self;
		int64_t a = integer(self);
		ASSERT(is_integer(args[0]));
		int64_t b = integer(args[0]);
		return value(a / b);
	}
	
	static VALUE integer_mod(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		if (num_args == 0)
			return self;
		int64_t a = integer(self);
		ASSERT(is_integer(args[0]));
		int64_t b = integer(args[0]);
		return value(a % b);
	}

	static VALUE integer_power(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		if (num_args == 0)
			return self;
		int64_t a = integer(self);
		ASSERT(is_integer(args[0]));
		int64_t b = integer(args[0]);
		int64_t v = pow(a,b);
		return value(v);
	}

	static VALUE integer_to_string(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
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
				throw_exception(new String("Unsupported base: %", value((int64_t)base)));
		}

		return value(new String(ss.str()));
	}
	
	static VALUE integer_lt(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		ASSERT_ARGS(num_args >= 1);
		return value(integer(self) < integer(args[0]));
	}
	
	static VALUE integer_lte(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		ASSERT_ARGS(num_args >= 1);
		return value(integer(self) <= integer(args[0]));
	}
	
	static VALUE integer_gt(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		ASSERT_ARGS(num_args >= 1);
		return value(integer(self) > integer(args[0]));
	}
	
	static VALUE integer_gte(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		ASSERT_ARGS(num_args >= 1);
		return value(integer(self) >= integer(args[0]));
	}
	
	static VALUE integer_sqrt(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		ASSERT_ARGS(num_args == 0);
		float v = sqrt(integer(self));
		return value(v);
	}
	
 	Object* integer_prototype() {
		static Object* ip = NULL;
		if (ip) return ip;
		ip = new(kMalloc) Object;
		ip->set_raw_s("name", new(kMalloc) String("Integer"));
		ip->set_raw_s("puts", FUNC(integer_puts));
		ip->set_raw_s("+", FUNC(integer_add));
		ip->set_raw_s("-", FUNC(integer_sub));
		ip->set_raw_s("*", FUNC(integer_mul));
		ip->set_raw_s("/", FUNC(integer_div));
		ip->set_raw_s("%", FUNC(integer_mod));
		ip->set_raw_s("**", FUNC(integer_power));
		VALUE to_string = FUNC(integer_to_string);
		ip->set_raw_s("to_string", to_string);
		ip->set_raw_s("inspect", to_string);
		ip->set_raw_s("<", FUNC(integer_lt));
		ip->set_raw_s("<=", FUNC(integer_lte));
		ip->set_raw_s(">", FUNC(integer_gt));
		ip->set_raw_s(">=", FUNC(integer_gte));
		ip->set_raw_s("sqrt", FUNC(integer_sqrt));
		return ip;
	}
}
