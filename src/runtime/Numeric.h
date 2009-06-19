#ifndef NUMERIC_H_VYDCMUQZ
#define NUMERIC_H_VYDCMUQZ

#include "Value.h"

namespace snow {
	VALUE numeric_add(VALUE self, uintx num_args, VALUE* args);
	VALUE numeric_sub(VALUE self, uintx num_args, VALUE* args);
	VALUE numeric_mul(VALUE self, uintx num_args, VALUE* args);
	VALUE numeric_div(VALUE self, uintx num_args, VALUE* args);
	VALUE numeric_mod(VALUE self, uintx num_args, VALUE* args);
	VALUE numeric_power(VALUE self, uintx num_args, VALUE* args);
	VALUE numeric_lt(VALUE self, uintx num_args, VALUE* args);
	VALUE numeric_lte(VALUE self, uintx num_args, VALUE* args);
	VALUE numeric_gt(VALUE self, uintx num_args, VALUE* args);
	VALUE numeric_gte(VALUE self, uintx num_args, VALUE* args);
}

#endif /* end of include guard: NUMERIC_H_VYDCMUQZ */
