#include "Float.h"
#include "Numeric.h"
#include "Function.h"
#include "SnowString.h"
#include <sstream>

namespace snow {	
	static Value float_to_i(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		ASSERT(self.is_float());
		intx a = (intx)floatnum(self.value());
		return value(a);
	}
	
	static Value float_to_f(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		ASSERT(self.is_float());
		return self;
	}
	
	static Value float_to_string(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		std::stringstream ss;
		ss << floatnum(self.value());
		return value(gc_new<String>(ss.str()));
	}
	
	Ptr<Object> float_prototype() {
		static Ptr<Object> proto;
		if (proto) return proto;
		proto = malloc_new<Object>();
		proto->set_raw("+", gc_new<Function>(numeric_add));
		proto->set_raw("-", gc_new<Function>(numeric_sub));
		proto->set_raw("*", gc_new<Function>(numeric_mul));
		proto->set_raw("/", gc_new<Function>(numeric_div));
		proto->set_raw("%", gc_new<Function>(numeric_mod));
		proto->set_raw("**", gc_new<Function>(numeric_power));
		proto->set_raw("<", gc_new<Function>(numeric_lt));
		proto->set_raw("<=", gc_new<Function>(numeric_lte));
		proto->set_raw(">", gc_new<Function>(numeric_gt));
		proto->set_raw(">=", gc_new<Function>(numeric_gte));
		
		Value to_string = gc_new<Function>(float_to_string);
		proto->set_raw("to_string", to_string);
		proto->set_raw("inspect", to_string);
		proto->set_raw("to_f", gc_new<Function>(float_to_f));
		proto->set_raw("to_i", gc_new<Function>(float_to_i));
		return proto;
	}
	
	#ifndef ARCH_IS_64_BIT
	bool is_float(VALUE val) {
		return object_cast<Float>(val) != NULL;
	}
	
	VALUE value(float val) {
		return gc_new<Float>(val).value();
	}
	
	float floatnum(VALUE val) {
		Ptr<Float> f = object_cast<Float>(val);
		ASSERT(f);
		return f->value();
	}
	#endif
}
