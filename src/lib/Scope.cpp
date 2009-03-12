#include "lib/Scope.h"
#include "lib/Function.h"

namespace snow {
	Scope::Scope(const Handle<Function>& func) : 
		ThinObject(scope_prototype()),
		m_Function(func)
	{
		m_Locals = new Array;
	}
	
	bool Scope::has_local(const std::string& name) const {
		if (name == "self") {
			return m_Self != NULL;
		} else {
			return m_LocalMap.find(name) != m_LocalMap.end();
		}
	}
	
	VALUE Scope::get_local(const std::string& name) const {
		if (name == "self" && m_Self != NULL)
			return m_Self;
		
		auto iter = m_LocalMap.find(name);
		if (iter == m_LocalMap.end())
			return nil();
		
		auto idx = iter->second;
		return m_Locals->get_by_index(idx);
	}
	
	VALUE Scope::set_local(const std::string& name, const ValueHandle& val) {
		if (name == "self") {
			error("Trying to set `self'!");
			TRAP();
		}
		
		auto iter = m_LocalMap.find(name);
		int64_t idx;
		if (iter == m_LocalMap.end()) {
			idx = m_Locals->length();
			m_LocalMap[name] = idx;
		} else {
			idx = iter->second;
		}

		return m_Locals->set_by_index(idx, val);
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