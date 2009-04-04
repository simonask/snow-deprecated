#include "Function.h"
#include "Scope.h"
#include "CompiledCode.h"

namespace snow {
	Function::Function(NativeFunctionPtr ptr) : m_ParentScope(NULL), m_LocalMap(NULL) {
		m_NativePtr = ptr;
	}
	
	Function::Function(const CompiledCode& code) : m_ParentScope(NULL) {
		m_LocalMap = code.local_map();
		m_Ptr = code.function_pointer();
	}
	
	Function::Function(const Function& other) : m_ParentScope(other.m_ParentScope), m_LocalMap(other.m_LocalMap) {
		m_Ptr = other.m_Ptr;
	}
	
	VALUE Function::call(VALUE self, const Handle<Array>& args) {
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
		// XXX: What if Array-allocation causes GC, and argument list has no other roots?
		if (is_native()) {
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
	
	Handle<Object>& function_prototype() {
		static Permanent<Object> fp;
		if (fp) return fp;
		fp = new Object;
		return fp;
	}
}
