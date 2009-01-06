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
	VALUE copy(VALUE obj);
	void destroy(VALUE obj);
	
	/*
		Return Object pointer if the value is an object, otherwise return the
		appropriate prototype.
	*/
	Object* object_for(VALUE value);
}


#endif /* end of include guard: RUNTIME_H_RUMHEGGF */
