#ifndef NUMERIC_CPP_VYDCMUQZ
#define NUMERIC_CPP_VYDCMUQZ

#include "Runtime.h"
#include "CppStackFrame.h"
#include <cmath>

namespace snow {
	Value numeric_add(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		if (args.size == 0)
			return self;
		
		ASSERT(self.is_numeric() && is_numeric(args.data[0]));
		
		if (self.is_float() || is_float(args.data[0])) {
			return value(floatnum(call_method(self, "to_f").value()) + floatnum(call_method(args.data[0], "to_f").value()));
		} else {
			return value(integer(self.value()) + integer(args.data[0]));
		}
	}
	
	Value numeric_sub(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		if (args.size == 0) {
			if (self.is_float()) {
				return value((float)0.0 - floatnum(self.value()));
			} else if (is_integer(self.value())) {
				return value(0 - integer(self.value()));
			}
		}
		
		ASSERT(self.is_numeric() && is_numeric(args.data[0]));
		
		if (self.is_float() || is_float(args.data[0])) {
			return value(floatnum(call_method(self, "to_f").value()) - floatnum(call_method(args.data[0], "to_f").value()));
		} else {
			return value(integer(self.value()) - integer(args.data[0]));
		}
	}
	
	Value numeric_mul(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		ASSERT_ARGS(args.size > 0)
		ASSERT(self.is_numeric() && is_numeric(args.data[0]));
		
		if (self.is_float() || is_float(args.data[0])) {
			return value(floatnum(call_method(self, "to_f").value()) * floatnum(call_method(args.data[0], "to_f").value()));
		} else {
			return value(integer(self.value()) * integer(args.data[0]));
		}
	}
		
	Value numeric_div(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		ASSERT_ARGS(args.size > 0)
		ASSERT(self.is_numeric() && is_numeric(args.data[0]));
		
		if (self.is_float() || is_float(args.data[0])) {
			return value(floatnum(call_method(self, "to_f").value()) / floatnum(call_method(args.data[0], "to_f").value()));
		} else {
			return value(integer(self.value()) / integer(args.data[0]));
		}
	}
	
	Value numeric_mod(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		ASSERT_ARGS(args.size > 0)
		ASSERT(self.is_numeric() && is_numeric(args.data[0]));
		
		if (self.is_float() || is_float(args.data[0])) {
			return value(fmodf(floatnum(call_method(self, "to_f").value()), floatnum(call_method(args.data[0], "to_f").value())));
		} else {
			return value(integer(self.value()) % integer(args.data[0]));
		}
	}
	
	Value numeric_power(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		ASSERT_ARGS(args.size > 0)
		ASSERT(self.is_numeric() && is_numeric(args.data[0]));
		
		if (self.is_float() || is_float(args.data[0])) {
			return value(powf(floatnum(call_method(self, "to_f").value()), floatnum(call_method(args.data[0], "to_f").value())));
		} else {
			// TODO: Use mod-exp algorithm
			return value((intx)pow(integer(self.value()), integer(args.data[0])));
		}
	}

	Value numeric_lt(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		ASSERT_ARGS(args.size > 0)
		ASSERT(self.is_numeric() && is_numeric(args.data[0]));
		
		if (self.is_float() || is_float(args.data[0])) {
			return value(floatnum(call_method(self, "to_f").value()) < floatnum(call_method(args.data[0], "to_f").value()));
		} else {
			return value(integer(self.value()) < integer(args.data[0]));
		}
	}

	Value numeric_lte(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		ASSERT_ARGS(args.size > 0)
		ASSERT(self.is_numeric() && is_numeric(args.data[0]));
		
		if (self.is_float() || is_float(args.data[0])) {
			return value(floatnum(call_method(self, "to_f").value()) <= floatnum(call_method(args.data[0], "to_f").value()));
		} else {
			return value(integer(self.value()) <= integer(args.data[0]));
		}
	}
	
	Value numeric_gt(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		ASSERT_ARGS(args.size > 0)
		ASSERT(self.is_numeric() && is_numeric(args.data[0]));
		
		if (self.is_float() || is_float(args.data[0])) {
			return value(floatnum(call_method(self, "to_f").value()) > floatnum(call_method(args.data[0], "to_f").value()));
		} else {
			return value(integer(self.value()) > integer(args.data[0]));
		}
	}

	Value numeric_gte(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		ASSERT_ARGS(args.size > 0)
		ASSERT(self.is_numeric() && is_numeric(args.data[0]));
		
		if (self.is_float() || is_float(args.data[0])) {
			return value(floatnum(call_method(self, "to_f").value()) >= floatnum(call_method(args.data[0], "to_f").value()));
		} else {
			return value(integer(self.value()) >= integer(args.data[0]));
		}
	}

}

#endif /* end of include guard: NUMERIC_CPP_VYDCMUQZ */
