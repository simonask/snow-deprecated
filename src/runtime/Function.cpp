#include "Function.h"
#include "Scope.h"
#include "SnowString.h"
#include "runtime/Arguments.h"

namespace snow {
	GC_ROOTS_IMPL(Function) {
		GC_SUPER(Object);
		GC_ROOT(m_ParentScope);
		GC_ROOT(m_LocalMap);
	}
	
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
	
	Value Function::call(const Value& self, const Arguments& args) {
		if (is_native()) {
			return m_NativePtr(self, args);
		} else {
			HandleScope _;
			Handle<Function> THIS = this;
			Handle<Array> arg_array = Array::copy(args.data, args.size);
			Handle<Scope> scope = gc_new<Scope>(THIS);
			scope->set_self(self);
			scope->set_arguments(arg_array);
			return call_in_scope(scope);
		}
	}
	
	Value Function::call_in_scope(const Ptr<Scope>& scope) {
		ASSERT(!is_native());
		return m_Ptr(scope.value());
	}

	static Value function_call(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		Ptr<Function> func = object_cast<Function>(self);
		ASSERT(func);
		return func->call(self, args);
	}

	static Value function_call_with_self(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		Handle<Function> func = object_cast<Function>(self);
		Handle<Array> extra_args = NULL;
		ASSERT(func);
		ASSERT_ARGS(args.size > 0);
		if (args.size > 1)
		{
			extra_args = Array::copy(&args.data[1], args.size-1);
		}
		else
		{
			extra_args = gc_new<Array>();
		}
		Arguments ar;
		ar.size = args.size - 1;
		ar.data = extra_args->data();
		return func->call(args.data[0], ar);
	}
	
	Ptr<Object> function_prototype() {
		static Ptr<Object> proto;
		if (proto) return proto;
		proto = malloc_new<Object>();
		return proto;
	}

	void function_prototype_init() {
		Ptr<Object> proto = function_prototype();
		proto->set_raw("name", gc_new<String>("Function"));
		Value call_handler = gc_new<Function>(function_call);
		proto->set_raw("__call__", call_handler);
		proto->set_raw("call", call_handler);
		proto->set_raw("call_with_self", gc_new<Function>(function_call_with_self));
	}
}
