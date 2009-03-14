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
		Handle<Scope> m_ParentScope;
		Handle<LocalMap> m_LocalMap;
		union {
			FunctionPtr m_Ptr;
			NativeFunctionPtr m_NativePtr;
		};
	public:
		Function(NativeFunctionPtr ptr);
		explicit Function(const CompiledCode& code);
		Function(const Function& other);
		VALUE call(const ValueHandle& self, const Handle<Array>& args);
		VALUE call(const ValueHandle& self, uint64_t num_args, ...);
		VALUE va_call(const ValueHandle& self, uint64_t num_args, va_list&);
		VALUE copy() const { return value((Object*)new Function(*this)); }
		
		const Handle<Scope>& parent_scope() const { return m_ParentScope; }
		void set_parent_scope(const Handle<Scope>& scope) { m_ParentScope = scope; }
		
		bool is_native() const { return !m_LocalMap; }
		const Handle<LocalMap>& local_map() const { return m_LocalMap; }
	};
}

#endif /* end of include guard: FUNCTION_H_ZAXPBYJ7 */
