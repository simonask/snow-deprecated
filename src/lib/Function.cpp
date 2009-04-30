#include "Function.h"
#include "Scope.h"
#include "CompiledCode.h"
#include "SnowString.h"

namespace snow {
	Function::Function(NativeFunctionPtr ptr) : Object(function_prototype()), m_ParentScope(NULL), m_LocalMap(NULL) {
		m_NativePtr = ptr;
	}
	
	Function::Function(const CompiledCode& code) : Object(function_prototype()), m_ParentScope(NULL) {
		m_LocalMap = code.local_map();
		m_Ptr = code.function_pointer();
	}
	
	Function::Function(const Function& other) : Object(function_prototype()), m_ParentScope(other.m_ParentScope), m_LocalMap(other.m_LocalMap) {
		m_Ptr = other.m_Ptr;
	}
	
	VALUE Function::call(VALUE _self, const Handle<Array>& args) {
		HandleScope _;
		ValueHandle self = _self;
		if (!self && m_ParentScope) {
			self = m_ParentScope->self();
		}

		if (is_native()) {
			return m_NativePtr(self, args->length(), args->data());
		} else {
			// TODO: Is it necessary to put Scope in a handle here?
			Handle<Scope> scope = new Scope(this);
			scope->set_self(self);
			scope->set_arguments(args);
			return call_in_scope(scope);
		}
	}
	
	VALUE Function::call(VALUE self, uint64_t num_args, ...) {
		va_list ap;
		va_start(ap, num_args);
		VALUE ret = va_call(self, num_args, ap);
		va_end(ap);
		return ret;
	}
	
	VALUE Function::va_call(VALUE self, uint64_t num_args, va_list& ap) {
		HandleScope _scope;
		if (is_native()) {
			// TODO: Put Array in a Local<>
			Array args(num_args);
			for (uint64_t i = 0; i < num_args; ++i) {
				args[i] = va_arg(ap, VALUE);
			}
			return call(self, &args);
		} else {
			Handle<Array> args = new Array(num_args);
			for (uint64_t i = 0; i < num_args; ++i) {
				(*args)[i] = va_arg(ap, VALUE);
			}
			return call(self, args);
		}
		
	}
	
	VALUE Function::call_in_scope(Scope* scope) {
		ASSERT(!is_native());
		return m_Ptr(scope);
	}


	static VALUE function_call_with_self(VALUE self, uint64_t num_args, VALUE* args) {
		Function* func = object_cast<Function>(self);
		ASSERT(func);
		ASSERT_ARGS(num_args > 0);
		Array* extra_args = NULL;
		if (num_args > 1)
			extra_args = new Array(&args[1], num_args-1);
		else
			extra_args = new Array;
		return func->call(args[0], extra_args);
	}
	
	Object* function_prototype() {
		static Object* proto = NULL;
		if (proto) return proto;
		proto = new(kMalloc) Object;
		proto->set_by_string("name", new String("Function"));
		proto->set_by_string("call_with_self", new(kMalloc) Function(function_call_with_self));
		return proto;
	}
}
