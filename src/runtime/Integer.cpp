#include "Integer.h"
#include "Numeric.h"
#include "Runtime.h"
#include "SnowString.h"
#include "Function.h"
#include "Exception.h"
#include <sstream>

#define FUNC new(kMalloc) Function //;

namespace snow {
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
	
	static VALUE integer_to_f(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		ASSERT_ARGS(num_args == 0);
		return value((float)integer(self));
	}
	
	static VALUE integer_to_i(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		ASSERT_ARGS(num_args == 0);
		return self;
	}
	
	Object* integer_prototype() {
		static Object* ip = NULL;
		if (ip) return ip;
		ip = new(kMalloc) Object;
		ip->set_raw_s("+", FUNC(numeric_add));
		ip->set_raw_s("-", FUNC(numeric_sub));
		ip->set_raw_s("*", FUNC(numeric_mul));
		ip->set_raw_s("/", FUNC(numeric_div));
		ip->set_raw_s("%", FUNC(numeric_mod));
		ip->set_raw_s("**", FUNC(numeric_power));
		ip->set_raw_s("<", FUNC(numeric_lt));
		ip->set_raw_s("<=", FUNC(numeric_lte));
		ip->set_raw_s(">", FUNC(numeric_gt));
		ip->set_raw_s(">=", FUNC(numeric_gte));
		
		VALUE to_string = FUNC(integer_to_string);
		ip->set_raw_s("to_string", to_string);
		ip->set_raw_s("inspect", to_string);
		ip->set_raw_s("to_f", FUNC(integer_to_f));
		ip->set_raw_s("to_i", FUNC(integer_to_i));
		return ip;
	}
}
