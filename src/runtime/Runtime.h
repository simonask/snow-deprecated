#ifndef RUNTIME_H_RUMHEGGF
#define RUNTIME_H_RUMHEGGF

#include "base/Basic.h"
#include "runtime/Value.h"

namespace snow {
	struct StackFrame;

	VALUE create_object(Object* prototype = NULL);
	VALUE create_string(const char* str);
	VALUE call(VALUE self, VALUE function_or_object, uint64_t num_args, ...);
	VALUE call_method(VALUE self, const char* name, uint64_t num_args, ...);
	VALUE get(VALUE self, VALUE sym_member);
	VALUE set(VALUE self, VALUE sym_member, VALUE val);
	bool equals(VALUE a, VALUE b);
	uint64_t get_object_id(VALUE val);
	
	void set_parent_scope(VALUE func, VALUE scope);
	void enter_scope(Scope* scope, StackFrame* frame);
	void leave_scope();
	StackFrame* get_current_stack_frame();
	void set_current_stack_frame(StackFrame* frame);
	void update_stack_frame(StackFrame* frame, Scope* scope);
	VALUE get_local(StackFrame* frame, VALUE symbol, bool quiet);
	VALUE set_local(StackFrame* frame, VALUE symbol, VALUE val);
	
	const char* value_to_string(VALUE obj);
	
	/*
		Return Object pointer if the value is an object, otherwise return the
		appropriate prototype.
	*/
	IObject* object_for(VALUE value);


	void print_stack_trace();
}


#endif /* end of include guard: RUNTIME_H_RUMHEGGF */
