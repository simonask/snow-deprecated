#include "lib/Scope.h"
#include "lib/Function.h"

namespace snow {
	Scope::Scope(const Handle<Function>& func) : 
		ThinObject(scope_prototype()),
		m_Function(func)
	{
		m_Locals = new Array;
	}
	
	bool Scope::has_local(const std::string& name) {
		if (name == "self") {
			return m_Self != NULL;
		} else {
			return local_map().find(name) != local_map().end();
		}
	}
	
	VALUE Scope::get_local(const std::string& name) {
		if (name == "self" && m_Self != NULL)
			return m_Self;
		
		auto iter = local_map().find(name);
		if (iter == local_map().end())
			return nil();
		
		auto idx = iter->second;
		return m_Locals->get_by_index(idx);
	}
	
	VALUE Scope::set_local(const std::string& name, const ValueHandle& val) {
		if (name == "self") {
			error("Trying to set `self'!");
			TRAP();
		}
		
		auto iter = local_map().find(name);
		int64_t idx;
		if (iter == local_map().end()) {
			idx = local_map().size();
			local_map()[name] = idx;
		} else {
			idx = iter->second;
		}

		return m_Locals->set_by_index(idx, val);
	}
	
	static Handle<Object> ScopePrototype = NULL;
	
	// TODO: Some more scope prototype functions.
	
	Handle<Object>& scope_prototype() {
		if (ScopePrototype)
			return ScopePrototype;
		ScopePrototype = new Object;
		return ScopePrototype;
	}
}