#ifndef FUNCTION_H_ZAXPBYJ7
#define FUNCTION_H_ZAXPBYJ7

#include "Object.h"
#include "Scope.h"

namespace snow {
	typedef VALUE(*FunctionPtr)(Scope*);
	
	Handle<Object>& function_prototype();
	
	class Function : public Object {
	private:
		FunctionPtr m_Ptr;
		Scope::LocalMap m_LocalMap;
		Handle<Scope> m_ParentScope;
	public:
		Function(FunctionPtr ptr = NULL) : Object(function_prototype()), m_Ptr(ptr) {}
		Function(const Function& other) : m_Ptr(other.m_Ptr), m_LocalMap(other.m_LocalMap), m_ParentScope(other.m_ParentScope) {}
		VALUE call(const Handle<Scope> scope);
		VALUE call(const ValueHandle& self, const Handle<Array>& args);
		VALUE va_call(const ValueHandle& self, uint64_t num_args, va_list&);
		VALUE copy() const { return value((Object*)new Function(*this)); }

		FunctionPtr function() const { return m_Ptr; }
		void set_function(FunctionPtr ptr) { m_Ptr = ptr; }
		
		const Handle<Scope>& parent_scope() const { return m_ParentScope; }
		void set_parent_scope(const Handle<Scope> scope) { m_ParentScope = scope; }
		
		Scope::LocalMap& local_map() { return m_LocalMap; }
		bool has_local(const std::string& name) const { return m_LocalMap.find(name) != m_LocalMap.end(); }
	};
}

#endif /* end of include guard: FUNCTION_H_ZAXPBYJ7 */
