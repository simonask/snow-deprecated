#include "lib/Scope.h"
#include "lib/Function.h"

namespace snow {
	Scope::Scope(const Handle<Function>& func) : 
		ThinObject(scope_prototype()),
		m_Function(func),
		m_LocalMap(NULL),
		m_Arguments(NULL),
		m_Locals(NULL),
		m_CallingScope(NULL)
	{
		if (func)
			m_LocalMap = func->local_map();
		
		size_t num_locals = m_LocalMap ? m_LocalMap->size() : 0;
		if (num_locals > 0) {
			VALUE nils[num_locals];
			for (size_t i = 0; i < num_locals; ++i) {
				nils[i] = nil();
			}
			m_Locals = new Array(nils, num_locals);
		}
	}
	
	void Scope::gc_mark() {
		ThinObject::gc_mark();
		// Necessary, since we don't use Handles...
		Garbage::mark(m_Self);
		Garbage::mark(m_Function);
		Garbage::mark(m_LocalMap);
		Garbage::mark(m_Arguments);
		Garbage::mark(m_Locals);
		Garbage::mark(m_CallingScope);
	}
	
	bool Scope::has_local(const std::string& name) {
		if (!m_LocalMap)
			return false;
		else
			return m_LocalMap->has_local(name);
	}
	
	VALUE Scope::get_local(const std::string& name) {
		if (name == "self" && m_Self)
			return m_Self;
			
		if (!has_local(name))
			return nil();
		
		return m_Locals->get_by_index(local_map()->local(name));
	}
	
	VALUE Scope::set_local(const std::string& name, const ValueHandle& val) {
		if (name == "self") {
			error("Trying to set `self'!");
			TRAP();
		}
		
		if (!m_LocalMap) {
			m_LocalMap = new LocalMap;
			m_Locals = new Array;
		}
		
		uint64_t idx;
		
		if (!has_local(name)) {
			idx = local_map()->define_local(name);
		} else {
			idx = local_map()->local(name);
		}

		return m_Locals->set_by_index(idx, val);
	}
	
	// TODO: Some more scope prototype functions.
	
	Handle<Object>& scope_prototype() {
		static Handle<Object> proto;
		if (proto) return proto;
		proto = new Object;
		return proto;
	}
}