#ifndef RUNTIME_H_RUMHEGGF
#define RUNTIME_H_RUMHEGGF

#include "base/Basic.h"
#include "runtime/Value.h"
#include "base/ThreadLocal.h"
#include "runtime/Symbol.h"

namespace snow {
	struct StackFrame;
	struct Arguments;
	
	Value call_with_arguments(const Value& self, const Value& function_or_object, const Arguments& args);
	Value call_with_arguments(const Value& self, const Value& function_or_object, uintx num_args, VALUE* args);
	Value get(const Value& self, Symbol sym_member);
	Value set(const Value& self, Symbol sym_member, const Value& val);
	bool equals(const Value& a, const Value& b);
	uintx get_object_id(const Value& val);
	
	
	void set_parent_scope(VALUE func, VALUE scope);
	void enter_scope(Scope* scope, StackFrame* frame);
	void leave_scope();
	StackFrame* get_current_stack_frame();
	ThreadLocal<StackFrame*>& get_current_stack_frames();
	void set_current_stack_frame(StackFrame* frame);
	void update_stack_frame(StackFrame* frame, Scope* scope);
	VALUE call_internal(VALUE self, VALUE function_or_object, uintx num_args, ...);
	VALUE get_internal(VALUE self, VALUE sym_member);
	VALUE set_internal(VALUE self, VALUE sym_member, VALUE val);
	VALUE get_local_internal(StackFrame* frame, VALUE symbol, bool quiet);
	VALUE set_local_internal(StackFrame* frame, VALUE symbol, VALUE val);
	
	const char* value_to_string(const Value& obj);
	
	/*
		Return Object pointer if the value is an object, otherwise return the
		appropriate prototype.
	*/
	Ptr<IObject> object_for(const Value& value);

	#ifdef DEBUG
	void print_stack_trace();
	#endif
	
	template <typename T>
	inline void copy_variadic_to_array(T* array, uintx offset) {}
	template <typename T, typename... Args>
	inline void copy_variadic_to_array(T* array, uintx offset, const Value& element, const Args&... a) {
		array[offset] = element.value();
		copy_variadic_to_array(array, offset+1, a...);
	}
	
	template <typename... Args>
	Value call(const Value& self, const Value& function_or_object, const Args&... a) {
		VALUE array[sizeof...(Args)];
		copy_variadic_to_array(array, 0, a...);
		return snow::call_with_arguments(self, function_or_object, sizeof...(Args), array);
	}
	
	template <typename... Args>
	Value call_method(const Value& self, Symbol method_name, const Args&... a) {
		Value function_or_object = snow::get(self, method_name);
		return snow::call(self, function_or_object, a...);
	}
}


#endif /* end of include guard: RUNTIME_H_RUMHEGGF */
