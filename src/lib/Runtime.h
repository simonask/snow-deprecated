#ifndef RUNTIME_H_RUMHEGGF
#define RUNTIME_H_RUMHEGGF

#include "Value.h"
#include "Function.h"
#include "StackFrame.h"

namespace snow {
	VALUE create_object(Object* prototype = NULL);
	VALUE create_function(FunctionPtr func);
	VALUE create_string(const char* str);
	VALUE create_float(double f);
	VALUE call(VALUE function_or_object, uint64_t num_args, ...);
	VALUE call_method(VALUE self, const char* message, uint64_t num_args, ...);
	VALUE send(VALUE obj, const char* message);
	VALUE copy(VALUE obj);
	void destroy(VALUE obj);
	
	void set_parent_scope(VALUE func, VALUE scope);
	void enter_scope(Scope* scope, StackFrame* frame);
	void leave_scope();
	StackFrame* get_current_stack_frame();
	VALUE get_local(StackFrame* frame, const char* local, bool quiet);
	
	const char* value_to_string(VALUE obj);
	
	/*
		Return Object pointer if the value is an object, otherwise return the
		appropriate prototype.
	*/
	Object* object_for(VALUE value);
}


#endif /* end of include guard: RUNTIME_H_RUMHEGGF */
