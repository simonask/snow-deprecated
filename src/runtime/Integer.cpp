#include "Integer.h"
#include "Numeric.h"
#include "Runtime.h"
#include "SnowString.h"
#include "Function.h"
#include "Exception.h"
#include <sstream>

namespace snow {
	static Value integer_to_string(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		std::stringstream ss;
		int base = 10;
		if (args.size > 0) {
			ASSERT(is_integer(args.data[0]));
			base = integer(args.data[0]);
		}

		switch (base) {
			case 10:
				ss << integer(self.value()); break;
			case 16:
				ss << "0x" << std::hex << integer(self.value()); break;
			/*case 8:
				ss << std::octal << integer(self); break;*/
			default:
				throw_exception(gc_new<String>("Unsupported base: %", value((intx)base)));
		}

		return value(gc_new<String>(ss.str()));
	}
	
	static Value integer_to_f(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		ASSERT_ARGS(args.size == 0);
		return value((float)integer(self.value()));
	}
	
	static Value integer_to_i(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		ASSERT_ARGS(args.size == 0);
		return self;
	}
	
	static Value integer_times(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		ASSERT_ARGS(args.size == 1);
		ASSERT_OBJECT(args.data[0], Function);
		Handle<Function> closure = object_cast<Function>(args.data[0]);
		
		if (integer(self.value()) <= 0) return self;
		for (intx i = 0; i < integer(self.value()); ++i)
			snow::call(NULL, closure, value((intx)i));
		
		return self;
	}
	
	Ptr<Object> integer_prototype() {
		static Ptr<Object> ip;
		if (ip) return ip;
		ip = malloc_new<Object>();
		ip->set_raw("+", gc_new<Function>(numeric_add));
		ip->set_raw("-", gc_new<Function>(numeric_sub));
		ip->set_raw("*", gc_new<Function>(numeric_mul));
		ip->set_raw("/", gc_new<Function>(numeric_div));
		ip->set_raw("%", gc_new<Function>(numeric_mod));
		ip->set_raw("**", gc_new<Function>(numeric_power));
		ip->set_raw("<", gc_new<Function>(numeric_lt));
		ip->set_raw("<=", gc_new<Function>(numeric_lte));
		ip->set_raw(">", gc_new<Function>(numeric_gt));
		ip->set_raw(">=", gc_new<Function>(numeric_gte));
		
		Value to_string = gc_new<Function>(integer_to_string);
		ip->set_raw("to_string", to_string);
		ip->set_raw("inspect", to_string);
		ip->set_raw("to_f", gc_new<Function>(integer_to_f));
		ip->set_raw("to_i", gc_new<Function>(integer_to_i));
		ip->set_raw("times", gc_new<Function>(integer_times));
		return ip;
	}
}
