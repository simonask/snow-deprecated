#ifndef RUNTIME_H_RUMHEGGF
#define RUNTIME_H_RUMHEGGF

#include "Value.h"
#include "Function.h"

namespace snot {
	VALUE create_object(Object* prototype = NULL);
	VALUE create_function(FunctionPtr func);
	VALUE create_string(const char* str);
	VALUE call(VALUE obj, VALUE self, uint64_t num_args, ...);
	VALUE send(VALUE obj, const char* message, uint64_t num_args, ...);
	void destroy(VALUE obj);
}


#endif /* end of include guard: RUNTIME_H_RUMHEGGF */
