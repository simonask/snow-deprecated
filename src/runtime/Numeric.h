#ifndef NUMERIC_H_VYDCMUQZ
#define NUMERIC_H_VYDCMUQZ

#include "Value.h"

namespace snow {
	Value numeric_add(const Value& self, const Arguments& args);
	Value numeric_sub(const Value& self, const Arguments& args);
	Value numeric_mul(const Value& self, const Arguments& args);
	Value numeric_div(const Value& self, const Arguments& args);
	Value numeric_mod(const Value& self, const Arguments& args);
	Value numeric_power(const Value& self, const Arguments& args);
	Value numeric_lt(const Value& self, const Arguments& args);
	Value numeric_lte(const Value& self, const Arguments& args);
	Value numeric_gt(const Value& self, const Arguments& args);
	Value numeric_gte(const Value& self, const Arguments& args);
}

#endif /* end of include guard: NUMERIC_H_VYDCMUQZ */
