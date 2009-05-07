#include "Runtime.h"
#include "Object.h"
#include "Integer.h"
#include "Nil.h"
#include "Boolean.h"
#include "SnowString.h"
#include "Float.h"
#include "Scope.h"
#include "Exception.h"
#include <stdarg.h>

namespace snow {
	VALUE create_object(Object* prototype) {
		return value(new Object(prototype));
	}
	
	VALUE create_string(const char* str) {
		return value(new String(str));
	}
	
	static VALUE va_call(VALUE self, VALUE function_or_object, uint64_t num_args, va_list& ap) {
		static const VALUE call_symbol = symbol("__call__");
		
		auto func = object_cast<Function>(function_or_object);
		if (func) {
			return func->va_call(self, num_args, ap);
		}

		VALUE call_handler = get(function_or_object, call_symbol);
		return object_for(call_handler)->va_call(self ? self : function_or_object, num_args, ap);
	}

	VALUE call_method(VALUE self, const char* str, uint64_t num_args, ...) {
		VALUE ret;
		va_list ap;
		va_start(ap, num_args);
		ret = va_call(self, get(self, symbol(str)), num_args, ap);
		va_end(ap);
		return ret;
	}
	
	VALUE call(VALUE self, VALUE function_or_object, uint64_t num_args, ...) {
		VALUE ret;
		va_list ap;
		va_start(ap, num_args);
		ret = va_call(self, function_or_object, num_args, ap);
		va_end(ap);
		return ret;
	}
	
	VALUE get(VALUE obj, VALUE member) {
		IObject* interface = object_for(obj);
		VALUE val = interface->get(obj, member);
		return val;
	}
	
	VALUE set(VALUE obj, VALUE member, VALUE val) {
		ASSERT(is_object(obj) && "Cannot set members of immediates!");
		return object_for(obj)->set(obj, member, val);
	}

	bool equals(VALUE a, VALUE b) {
		static const VALUE equal_sign_symbol = symbol("=");
		return eval_truth(snow::call(a, get(a, equal_sign_symbol), 1, b));
	}

	uint64_t get_object_id(VALUE val) {
		if (!is_object(val)) {
			return reinterpret_cast<uint64_t>(val);
		} else {
			return object_cast(val)->id() << 0x10;
		}
	}
	
	void set_parent_scope(VALUE func, VALUE scope) {
		ASSERT_OBJECT(func, Function);
		object_cast<Function>(func)->set_parent_scope(object_cast<Scope>(scope));
	}
	
	static StackFrame* current_frame = NULL;
	
	StackFrame* get_current_stack_frame() {
		return current_frame;
	}

	void set_current_stack_frame(StackFrame* frame) {
		current_frame = frame;
	}
	
	void enter_scope(Scope* scope, StackFrame* frame) {
		if (scope)
			update_stack_frame(frame, scope);
		
		frame->previous = current_frame;
		current_frame = frame;
	}
	
	void leave_scope() {
		ASSERT(current_frame);
		current_frame = current_frame->previous;
	}


	void update_stack_frame(StackFrame* frame, Scope* scope) {
		frame->scope = scope;
		if (scope->locals())
			frame->locals = scope->locals()->data();
		else
			frame->locals = NULL;
		if (scope->arguments()) {
			frame->args = scope->arguments()->data();
			frame->num_args = scope->arguments()->length();
		} else {
			frame->args = NULL;
			frame->num_args = 0;
		}
		frame->self = scope->self();
		frame->it = scope->arguments() && scope->arguments()->length() > 0 ? scope->arguments()->get_by_index(0) : nil();
	}
	
	VALUE get_local(StackFrame* frame, VALUE name, bool quiet) {
		Scope* scope = frame->scope;
		while (scope) {
			/*
			// When debugging locals, this is nice:
			for (auto iter = scope->local_map()->map().begin(); iter != scope->local_map()->map().end(); ++iter) {
				debug("local: %-10s at %d: 0x%llx", iter->first.c_str(),
					iter->second,
					scope->locals()->get_by_index(iter->second));
			}
			*/
			if (scope->has_local(name)) {
				VALUE val = scope->get_local(name);
				return val;
			}
			scope = scope->function() ? (Scope*)scope->function()->parent_scope() : NULL;
		}
		throw_exception(new String("Undefined local `%'", name));
		return nil();
	}

	VALUE set_local(StackFrame* frame, VALUE name, VALUE val) {
		return frame->scope->set_local(name, val);
	}
	
	const char* value_to_string(VALUE obj) {
		VALUE returned = call_method(obj, "to_string", 0);
		
		ASSERT_OBJECT(returned, String);
		
		auto string = object_cast<String>(returned);
		
		return (const char*)(*string);
	}
	
	IObject* object_for(VALUE obj) {
		if (is_object(obj)) {
			return object_cast<IObject>(obj);
		}
		if (is_integer(obj))
			return integer_prototype();
		if (is_boolean(obj))
			return boolean_prototype();
		if (is_nil(obj))
			return nil_prototype();
		if (is_symbol(obj))
			return symbol_prototype();
		if (is_float(obj))
			return float_prototype();
		
		return nil_prototype();
	}

	void print_stack_trace() {
		StackFrame* frame = get_current_stack_frame();
		while (frame) {
			printf("%s:%llu\t%s(", frame->file, frame->line, frame->funcname);
			for (size_t i = 0; i < frame->num_args && frame->args; ++i) {
				printf("0x%llx", frame->args[i]);
				if (i != frame->num_args-1)
					printf(", ");
			}
			printf(")\n");
			frame = frame->previous;
		}
	}
}
