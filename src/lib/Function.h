#ifndef FUNCTION_H_ZAXPBYJ7
#define FUNCTION_H_ZAXPBYJ7

#include "Object.h"
#include "Scope.h"

namespace snow {
	typedef VALUE(*FunctionPtr)(Scope*);
	
	Handle<Object>& function_prototype();
	
	class Function : public Object {
	private:
		typedef std::unordered_map<std::string, uint64_t, SuperFastHash> LocalMap;

		FunctionPtr m_Ptr;
		LocalMap* m_LocalMap;
		uint64_t m_NumLocals;
		Handle<Scope> m_ParentScope;
	public:
		Function(FunctionPtr ptr = NULL) : Object(function_prototype()), m_Ptr(ptr) {}
		Function(const Function& other) : m_Ptr(other.m_Ptr), m_LocalMap(other.m_LocalMap), m_NumLocals(other.m_NumLocals), m_ParentScope(other.m_ParentScope) {}
		VALUE call(const ValueHandle& self, const Handle<Array>& args);
		VALUE va_call(const ValueHandle& self, uint64_t num_args, va_list&);
		VALUE copy() const { return value((Object*)new Function(*this)); }

		FunctionPtr function() const { return m_Ptr; }
		void set_function(FunctionPtr ptr) { m_Ptr = ptr; }
		
		uint64_t num_locals() const { return m_NumLocals; }
		
		const Handle<Scope>& parent_scope() const { return m_ParentScope; }
		void set_parent_scope(const Handle<Scope> scope) { m_ParentScope = scope; }
	};
}

#endif /* end of include guard: FUNCTION_H_ZAXPBYJ7 */
