#include "Function.h"

namespace snow {
	static Handle<Object> FunctionPrototype = NULL;
	
	VALUE Function::call(const ValueHandle& self, const Handle<Array>& args) {
		Handle<Scope> scope = new Scope(this);
		scope->set_self(self);
		scope->set_arguments(args);
		return m_Ptr(scope);
	}
	
	VALUE Function::va_call(const ValueHandle& self, uint64_t num_args, va_list& ap) {
		Handle<Array> args = new Array(num_args);
		for (uint64_t i = 0; i < num_args; ++i) {
			args->set_by_index(i, va_arg(ap, VALUE));
		}
		
		return call(self, args);
	}
	
	Handle<Object>& function_prototype() {
		if (FunctionPrototype)
			return FunctionPrototype;
		Object* fp = new Object;
		FunctionPrototype = fp;
		return FunctionPrototype;
	}
}