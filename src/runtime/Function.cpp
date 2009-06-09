#include "Function.h"
#include "Scope.h"
#include "SnowString.h"

namespace snow {
	Function::Function(NativeFunctionPtr ptr) : Object(function_prototype()), m_ParentScope(NULL), m_LocalMap(NULL), m_IsNative(true) {
		m_NativePtr = ptr;
	}
	
	Function::Function(const CompiledCode& code) : Object(function_prototype()), m_ParentScope(NULL), m_IsNative(false) {
		m_LocalMap = code.local_map();
		m_Ptr = code.function_pointer();
	}
	
	Function::Function(const Function& other) : Object(function_prototype()), m_ParentScope(other.m_ParentScope), m_LocalMap(other.m_LocalMap), m_IsNative(other.m_IsNative) {
		m_Ptr = other.m_Ptr;
	}
	
	VALUE Function::call_with_arguments(VALUE _self, const Array& _args) {
		HandleScope _;
		Handle<Function> THIS = this;
		ValueHandle self = _self;
		if (!self && m_ParentScope) {
			self = THIS->m_ParentScope->self();
		}

		Garbage::fence();

		if (is_native()) {
			return THIS->m_NativePtr(self, _args.length(), _args.data());
		} else {
			Handle<Array> args = Array::copy(_args);
			Handle<Scope> scope = gc_new<Scope>(THIS);
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
	
	VALUE Function::va_call(VALUE _self, uint64_t num_args, va_list& ap) {
		HandleScope _;
		VALUE real_args[num_args];
		for (uint64_t i = 0; i < num_args; ++i) {
			real_args[i] = va_arg(ap, VALUE);
		}

		Local<Array> args;
		L(args).set_reference(real_args, num_args);

		return call_with_arguments(_self, args);
	}
	
	VALUE Function::call_in_scope(Scope* scope) {
		ASSERT(!is_native());
		return m_Ptr(scope);
	}

	static VALUE function_call(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		Function* func = object_cast<Function>(self);
		ASSERT(func);
		Local<Array> arguments;
		L(arguments).set_reference(args, num_args);
		return func->call_with_arguments(self, arguments);
	}

	static VALUE function_call_with_self(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		Handle<Function> func = object_cast<Function>(self);
		Handle<Array> extra_args = NULL;
		ASSERT(func);
		ASSERT_ARGS(num_args > 0);
		if (num_args > 1)
		{
			extra_args = Array::copy(&args[1], num_args-1);
		}
		else
		{
			extra_args = gc_new<Array>();
		}
		return func->call_with_arguments(args[0], *extra_args);
	}
	
	Object* function_prototype() {
		static Object* proto = NULL;
		if (proto) return proto;
		proto = malloc_new<Object>();
		return proto;
	}

	void function_prototype_init() {
		Object* proto = function_prototype();
		proto->set_raw_s("name", gc_new<String>("Function"));
		VALUE call_handler = gc_new<Function>(function_call);
		proto->set_raw_s("__call__", call_handler);
		proto->set_raw_s("call", call_handler);
		proto->set_raw_s("call_with_self", gc_new<Function>(function_call_with_self));
	}
}
