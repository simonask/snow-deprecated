#include "Runtime.h"
#include "Object.h"
#include "Integer.h"
#include "Nil.h"
#include "SnowString.h"
#include <stdarg.h>

namespace snow {
	VALUE create_object(Object* prototype) {
		return value(new Object(prototype));
	}
	
	VALUE create_string(const char* str) {
		return value(new String(str));
	}
	
	VALUE create_float(double d) {
		return nil();
	}
	
	static VALUE va_call(VALUE self, VALUE function_or_object, uint64_t num_args, va_list& ap) {
		return object_for(function_or_object)->va_call(self, num_args, ap);
	}
	
	VALUE call(VALUE self, VALUE function_or_object, uint64_t num_args, ...) {
		VALUE ret;
		va_list ap;
		va_start(ap, num_args);
		ret = va_call(self, function_or_object, num_args, ap);
		va_end(ap);
		return ret;
	}
	
	VALUE get(VALUE obj, const char* member) {
		return object_for(obj)->get(member);
	}
	
	VALUE set(VALUE obj, const char* member, VALUE val) {
		ASSERT(is_object(obj) && "Cannot set members of immediates!");
		return object_for(obj)->set(member, val);
	}
	
	void set_parent_scope(VALUE func, VALUE scope) {
		ASSERT_OBJECT(func, Function);
		object_cast<Function>(func)->set_parent_scope(object_cast<Scope>(scope));
	}
	
	static StackFrame* current_frame = NULL;
	
	void enter_scope(Scope* scope, StackFrame* frame) {
		frame->scope = scope;
		if (scope->locals()) {
			scope->locals()->freeze();
			frame->locals = scope->locals()->data();
		}
		frame->arguments = scope->arguments()->data();
		
		frame->previous = current_frame;
		current_frame = frame;
	}
	
	void leave_scope() {
		if (current_frame) {
			if (current_frame->scope->locals())
				current_frame->scope->locals()->unfreeze();
			current_frame = current_frame->previous;
		} else {
			error("Leaving void scope.");
			TRAP();
		}
	}
	
	StackFrame* get_current_stack_frame() {
		return current_frame;
	}
	
	VALUE get_local(StackFrame* frame, const char* name, bool quiet) {
		Handle<Scope> scope = frame->scope;
		while (scope) {
/*			debug("looking for `%s' in scope 0x%llx", name, scope.value());
			for each (iter, scope->local_map()) {
				debug("local: %-10s at %d: 0x%llx", iter->first.c_str(), iter->second, scope->locals()->get_by_index(iter->second));
			}*/
			if (scope->has_local(name)) {
				VALUE val = scope->get_local(name);
				return val;
			}
			scope = scope->function() ? scope->function()->parent_scope() : Handle<Scope>(NULL);
		}
		error("Undefined local: `%s'", name);
		TRAP();
		return nil();
	}
	
	const char* value_to_string(VALUE obj) {
		VALUE returned = call(obj, get(obj, "to_string"), 0);
		
		ASSERT_OBJECT(returned, String);
		
		auto string = object_cast<String>(returned);
		
		return (const char*)(*string);
	}
	
	Object* object_for(VALUE obj) {
		switch (value_type(obj)) {
			case kObjectType:
				// Safeguard: NULL pointers become 'nil'
				return obj ? object_cast(obj) : (Object*)nil_prototype();
			case kEvenIntegerType:
			case kOddIntegerType:
				return integer_prototype();
			default:
			case kSpecialType:
				switch (special_value(obj)) {
					case kTrue:
					case kFalse:
//						return boolean_prototype();
						break;
					default:
					case kNil:
						return nil_prototype();
				}
				break;
		}

		return nil_prototype();
	}
}