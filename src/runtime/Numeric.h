#ifndef NUMERIC_H_VYDCMUQZ
#define NUMERIC_H_VYDCMUQZ

#include "Value.h"

namespace snow {
	VALUE numeric_add(VALUE self, uint64_t num_args, VALUE* args);
	VALUE numeric_sub(VALUE self, uint64_t num_args, VALUE* args);
	VALUE numeric_mul(VALUE self, uint64_t num_args, VALUE* args);
	VALUE numeric_div(VALUE self, uint64_t num_args, VALUE* args);
	VALUE numeric_mod(VALUE self, uint64_t num_args, VALUE* args);
	VALUE numeric_power(VALUE self, uint64_t num_args, VALUE* args);
	VALUE numeric_lt(VALUE self, uint64_t num_args, VALUE* args);
	VALUE numeric_lte(VALUE self, uint64_t num_args, VALUE* args);
	VALUE numeric_gt(VALUE self, uint64_t num_args, VALUE* args);
	VALUE numeric_gte(VALUE self, uint64_t num_args, VALUE* args);
}

#endif /* end of include guard: NUMERIC_H_VYDCMUQZ */
