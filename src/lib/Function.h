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
		RefPtr<CompiledCode> m_Code;
		FunctionPtr m_Ptr;
		Handle<Scope> m_ParentScope;
	public:
		Function(FunctionPtr ptr = NULL) : Object(function_prototype()), m_Ptr(ptr) {}
		Function(RefPtr<CompiledCode> code);
		Function(const Function& other) :  m_Code(other.m_Code), m_Ptr(other.m_Ptr), m_ParentScope(other.m_ParentScope) {}
		VALUE call(const Handle<Scope> scope);
		VALUE call(const ValueHandle& self, const Handle<Array>& args);
		VALUE va_call(const ValueHandle& self, uint64_t num_args, va_list&);
		VALUE copy() const { return value((Object*)new Function(*this)); }

		FunctionPtr function() const { return m_Ptr; }
		void set_function(FunctionPtr ptr) { m_Ptr = ptr; }
		
		RefPtr<CompiledCode> code() const { return m_Code; }
		
		const Handle<Scope>& parent_scope() const { return m_ParentScope; }
		void set_parent_scope(const Handle<Scope>& scope) { m_ParentScope = scope; }
		
		Handle<LocalMap>& local_map() { return m_Code->local_map(); }
		bool has_local(const std::string& name) { return local_map()->has_local(name); }
	};
}

#endif /* end of include guard: FUNCTION_H_ZAXPBYJ7 */
