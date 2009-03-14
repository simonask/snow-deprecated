#include "Function.h"
#include "Scope.h"
#include "CompiledCode.h"

namespace snow {
	static Handle<Object> FunctionPrototype = NULL;
	
	Function::Function(RefPtr<CompiledCode> code) : Object(function_prototype()), m_Code(code), m_Ptr(code->function_pointer())
	{
	}
	
	VALUE Function::call(const Handle<Scope> scope) {
		return m_Ptr(scope);
	}
	
	VALUE Function::call(const ValueHandle& self, const Handle<Array>& args) {
		Handle<Scope> scope = new Scope(this);
		scope->set_self(self);
		scope->set_arguments(args);
		return call(scope);
	}
	
	VALUE Function::va_call(const ValueHandle& self, uint64_t num_args, va_list& ap) {
		Handle<Array> args = new Array(num_args);
		for (uint64_t i = 0; i < num_args; ++i) {
			(*args)[i] = va_arg(ap, VALUE);
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