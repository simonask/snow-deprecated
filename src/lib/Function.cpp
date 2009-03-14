#include "Function.h"
#include "Scope.h"
#include "CompiledCode.h"

namespace snow {
	static Handle<Object> FunctionPrototype = NULL;
	
	Function::Function(NativeFunctionPtr ptr) {
		m_NativePtr = ptr;
	}
	
	Function::Function(const CompiledCode& code) {
		m_LocalMap = code.local_map();
		m_Ptr = code.function_pointer();
	}
	
	Function::Function(const Function& other) : m_ParentScope(other.m_ParentScope), m_LocalMap(other.m_LocalMap) {
		m_Ptr = other.m_Ptr;
	}
	
	VALUE Function::call(const ValueHandle& self, const Handle<Array>& args) {
		if (is_native()) {
			VALUE _self = self;
			if (!_self && m_ParentScope) {
				_self = m_ParentScope->self();
			}
			return m_NativePtr(_self, args->length(), args->data());
		} else {
			Handle<Scope> scope = new Scope(this);
			scope->set_self(self);
			scope->set_arguments(args);
			return m_Ptr(scope);
		}
	}
	
	VALUE Function::call(const ValueHandle& self, uint64_t num_args, ...) {
		va_list ap;
		va_start(ap, num_args);
		VALUE ret = va_call(self, num_args, ap);
		va_end(ap);
		return ret;
	}
	
	VALUE Function::va_call(const ValueHandle& self, uint64_t num_args, va_list& ap) {
		// XXX: What if Array-allocation causes GC, and argument list has no other roots?
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