#include "lib/Scope.h"
#include "lib/Function.h"

namespace snow {
	Scope::Scope(const Handle<Function>& func) : 
		Object(scope_prototype()),
		m_Function(func)
	{
		m_Locals = new Array(m_Function->num_locals());
	}
	
	
	static Handle<Object> ScopePrototype;
	
	// TODO: Some more scope prototype functions.
	
	Handle<Object>& scope_prototype() {
		if (ScopePrototype)
			goto done;
		ScopePrototype = new Object;
done:
		return ScopePrototype;
	}
}