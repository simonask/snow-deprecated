#include "Runtime.h"
#include "Object.h"
#include "Integer.h"
#include "Nil.h"
#include "Boolean.h"
#include "SnowString.h"
#include "Float.h"
#include "Scope.h"
#include "Exception.h"
#include "StackFrame.h"
#include "Arguments.h"
#include <stdarg.h>

namespace snow {
	static Value va_call(const Value& self, const Value& function_or_object, uintx num_args, va_list& ap) {
		VALUE args[num_args];
		for (uintx i = 0; i < num_args; ++i) {
			args[i] = va_arg(ap, VALUE);
		}
		
		return call_with_arguments(self, function_or_object, num_args, args);
	}
	
	VALUE call_internal(VALUE self, VALUE function_or_object, uintx num_args, ...) {
		VALUE ret;
		va_list ap;
		va_start(ap, num_args);
		ret = va_call(self, function_or_object, num_args, ap).value();
		va_end(ap);
		return ret;
	}
	
	Value call_with_arguments(const Value& self, const Value& function_or_object, uintx num_args, VALUE* args_data) {
		Arguments args;
		args.data = args_data;
		args.size = num_args;
		return call_with_arguments(self, function_or_object, args);
	}
	
	Value call_with_arguments(const Value& self, const Value& function_or_object, const Arguments& args) {
		static const Symbol call_symbol("__call__");
		HandleScope _;
		Handle<Function> func = object_cast<Function>(function_or_object);
		if (func) {
			return func->call(self, args);
		}

		ValueHandle call_handler = get(function_or_object, call_symbol);
		return object_for(call_handler)->call(self ? self : function_or_object, args);
	}
	
	Value get(const Value& self, Symbol member) {
		return get_internal(self.value(), member);
	}
	
	Value set(const Value& self, Symbol member, const Value& val) {
		return set_internal(self.value(), member, val.value());
	}
	
	VALUE get_internal(VALUE _object, VALUE _member) {
		HandleScope _;
		ValueHandle object = _object;
		ValueHandle member = _member;
		Handle<IObject> interface = object_for(object);
		ValueHandle val = interface->get(object, member);
		return val.value();
	}
	
	VALUE set_internal(VALUE obj, VALUE member, VALUE val) {
		ASSERT(is_object(obj) && "Cannot set members of immediates!");
		return object_for(obj)->set(obj, member, val).value();
	}

	bool equals(const Value& a, const Value& b) {
		static const Symbol equal_sign_symbol("=");
		return eval_truth(snow::call(a, get(a, equal_sign_symbol), b).value());
	}

	uintx get_object_id(const Value& val) {
		if (!val.is_object()) {
			return reinterpret_cast<uintx>(val.value());
		} else {
			return object_cast(val)->id() << 0x10;
		}
	}
	
	void set_parent_scope(VALUE func, VALUE scope) {
		ASSERT_OBJECT(func, Function);
		object_cast<Function>(func)->set_parent_scope(object_cast<Scope>(scope));
	}
	
	static ThreadLocal<StackFrame*> current_frame = NULL;
	
	StackFrame* get_current_stack_frame() {
		return current_frame;
	}

	ThreadLocal<StackFrame*>& get_current_stack_frames() {
		return current_frame;
	}

	void set_current_stack_frame(StackFrame* frame) {
		current_frame = frame;
	}
	
	void enter_scope(Scope* scope, StackFrame* frame) {
		ASSERT(frame != current_frame);
		update_stack_frame(frame, scope);
		
		// nullify temporaries
		for (size_t i = 0; i < frame->num_temporaries; ++i)
		{
			frame->temporaries[i] = NULL;
		}
		
		frame->previous = current_frame;
		current_frame = frame;
	}
	
	void leave_scope() {
		ASSERT(current_frame);
		current_frame = L(current_frame)->previous;
	}


	void update_stack_frame(StackFrame* frame, Scope* scope) {
		frame->scope = scope;
		if (scope)
		{
			if (scope->locals())
				frame->locals = scope->locals()->data();
			else
				frame->locals = NULL;
			if (scope->arguments()) {
				scope->arguments()->preallocate(scope->local_map()->num_arguments());
				frame->args = scope->arguments()->data();
				frame->num_args = scope->arguments()->length();
			} else {
				frame->args = NULL;
				frame->num_args = 0;
			}
			frame->self = scope->self().value();
			frame->it = scope->arguments() && scope->arguments()->length() > 0 ? scope->arguments()->get_by_index(0).value() : nil();
		} else {
			frame->locals = NULL;
			frame->args = NULL;
			frame->num_args = 0;
			frame->self = NULL;
			frame->it = nil();
		}
	}
	
	VALUE get_local_internal(StackFrame* frame, VALUE name, bool quiet) {
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
				VALUE val = scope->get_local(name).value();
				return val;
			}
			scope = scope->function() ? (Scope*)scope->function()->parent_scope().value() : NULL;
		}
		throw_exception(gc_new<String>("Undefined local `%'", name));
		return nil();
	}

	VALUE set_local_internal(StackFrame* frame, VALUE name, VALUE val) {
		return frame->scope->set_local(name, val).value();
	}
	
	const char* value_to_string(const Value& obj) {
		Value returned = call_method(obj, "to_string");
		
		ASSERT_OBJECT(returned, String);
		
		auto string = object_cast<String>(returned);
		
		return string->c_str();
	}
	
	Ptr<IObject> object_for(const Value& obj) {
		if (obj.is_object()) {
			return object_cast<IObject>(obj);
		}
		if (obj.is_integer())
			return integer_prototype();
		if (obj.is_boolean())
			return boolean_prototype();
		if (obj.is_nil())
			return nil_prototype();
		if (obj.is_symbol())
			return symbol_prototype();
		if (obj.is_float())
			return float_prototype();
		
		return nil_prototype();
	}

	void print_stack_trace() {
		StackFrame* frame = get_current_stack_frame();
		while (frame) {
			printf("%s:%u\t%s(", frame->file, frame->line, frame->funcname);
			for (size_t i = 0; i < frame->num_args && frame->args; ++i) {
				printf("0x%lx", frame->args[i]);
				if (i != frame->num_args-1)
					printf(", ");
			}
			printf(")\n");
			frame = frame->previous;
		}
	}
}
