#ifndef RUNTIME_H_RUMHEGGF
#define RUNTIME_H_RUMHEGGF

#include "Value.h"
#include "Function.h"

namespace snot {
	VALUE create_object(Object* prototype = NULL);
	VALUE create_function(FunctionPtr func);
	VALUE call(VALUE obj, VALUE self, uint64_t num_args, ...);
	VALUE send(VALUE obj, const char* message, uint64_t num_args, ...);
}


#endif /* end of include guard: RUNTIME_H_RUMHEGGF */
