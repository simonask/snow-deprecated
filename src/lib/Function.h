#ifndef FUNCTION_H_ZAXPBYJ7
#define FUNCTION_H_ZAXPBYJ7

#include "Object.h"
#include "LocalMap.h"
#include "CompiledCode.h"

namespace snow {
	class Scope;
	class Array;
	
	Handle<Object>& function_prototype();
	
	class Function : public Object {
	private:
		Scope* m_ParentScope;
		LocalMap* m_LocalMap;
		union {
			FunctionPtr m_Ptr;
			NativeFunctionPtr m_NativePtr;
		};

		GC_ROOTS { GC_SUPER(Object); GC_ROOT(m_ParentScope); GC_ROOT(m_LocalMap); }
	public:
		Function(NativeFunctionPtr ptr);
		explicit Function(const CompiledCode& code);
		Function(const Function& other);
		VALUE call(VALUE self, const Handle<Array>& args);
		VALUE call(VALUE self, uint64_t num_args, ...);
		VALUE va_call(VALUE self, uint64_t num_args, va_list&);
		VALUE call_in_scope(Scope* scope);
		VALUE copy() const { return value((Object*)new Function(*this)); }
		
		Scope* parent_scope() const { return m_ParentScope; }
		void set_parent_scope(Scope* scope) { m_ParentScope = scope; }
		
		bool is_native() const { return !m_LocalMap; }
		LocalMap* local_map() const { return m_LocalMap; }
	};
}

#endif /* end of include guard: FUNCTION_H_ZAXPBYJ7 */
