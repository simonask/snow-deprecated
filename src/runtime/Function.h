#ifndef FUNCTION_H_ZAXPBYJ7
#define FUNCTION_H_ZAXPBYJ7

#include "Object.h"
#include "LocalMap.h"
#include "codegen/CompiledCode.h"

namespace snow {
	class Scope;
	class Array;
	
	Ptr<Object> function_prototype();
	void function_prototype_init();
	
	class Function : public Object {
	private:
		Ptr<Scope> m_ParentScope;
		Ptr<LocalMap> m_LocalMap;
		const bool m_IsNative;
		union {
			FunctionPtr m_Ptr;
			NativeFunctionPtr m_NativePtr;
		};

		GC_ROOTS;
	public:
		NO_INIT;
		Function(NativeFunctionPtr ptr);
		explicit Function(const CompiledCode& code);
		Function(const Function& other);
		Value call(const Value& self, const Arguments&);
		Value call_in_scope(const Ptr<Scope>& scope);
		Ptr<IObject> copy() const { return new Function(*this); }
		
		Ptr<Scope> parent_scope() const { return m_ParentScope; }
		void set_parent_scope(const Ptr<Scope>& scope) { m_ParentScope = scope; }
		
		bool is_native() const { return m_IsNative; }
		Ptr<LocalMap> local_map() const { return m_LocalMap; }
	};
}

#endif /* end of include guard: FUNCTION_H_ZAXPBYJ7 */
