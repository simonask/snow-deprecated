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
	
	VALUE create_function(FunctionPtr ptr) {
		return value(new Function(ptr));
	}
	
	VALUE create_string(const char* str) {
		return value(new String(str));
	}
	
	VALUE create_float(double d) {
		return nil();
	}
	
	static VALUE va_call(VALUE function_or_object, VALUE self, uint64_t num_args, va_list& ap) {
		if (!is_object(function_or_object))
			return function_or_object;
		return object_for(function_or_object)->va_call(self, num_args, ap);
	}
	
	VALUE call(VALUE function_or_object, uint64_t num_args, ...) {
		VALUE ret;
		va_list ap;
		va_start(ap, num_args);
		ret = va_call(function_or_object, NULL, num_args, ap);
		va_end(ap);
		return ret;
	}
	
	VALUE call_method(VALUE self, const char* message, uint64_t num_args, ...) {
		VALUE ret;
		va_list ap;
		va_start(ap, num_args);
		VALUE function_or_object = send(object_for(self), message);
		if (!eval_truth(function_or_object))
			warn("METHOD %s UNDEFINED on object 0x%llx (self: 0x%llx)!", message, object_for(self), self);
		ret = va_call(function_or_object, self, num_args, ap);
		va_end(ap);
		return ret;
	}
	
	VALUE send(VALUE obj, const char* message) {
		return object_for(obj)->get(message);
	}
	
	VALUE copy(VALUE obj) {
		if (is_object(obj))
			return object_cast<Object>(obj)->copy();
		else
			return obj;
	}
	
	void destroy(VALUE _obj) {
		if (is_object(_obj)) {
			Object* obj = object_cast<Object>(_obj);
			
			VALUE finalize = send(obj, "finalize");
			call(finalize, 0);
			
			for (auto iter = iterate(obj->members()); iter; ++iter) {
				destroy(iter->second);
			}
			
			delete obj;
		}
	}
	
	void set_parent_scope(VALUE func, VALUE scope) {
		ASSERT_OBJECT(func, Function);
		object_cast<Function>(func)->set_parent_scope(object_cast<Scope>(scope));
	}
	
	static StackFrame* current_scope = NULL;
	
	void enter_scope() {
	}
	
	void leave_scope() {
		if (current_scope)
			current_scope = current_scope->previous;
		else
			warn("Leaving void scopse.");
	}
	
	StackFrame* get_current_stack_frame() {
		return current_scope;
	}
	
	VALUE get_local(StackFrame* frame, const char* _local, bool quiet) {
		// XXX: TODO
		return NULL;
	}
	
	const char* value_to_string(VALUE obj) {
		VALUE returned = call_method(obj, "to_string", 0);
		
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