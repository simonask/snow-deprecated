#include "runtime/Scope.h"
#include "runtime/Function.h"
#include "runtime/SnowString.h"
#include "runtime/Exception.h"

namespace snow {
	void Scope::initialize(const Ptr<Function>& func) {
		HandleScope _;
		Handle<Scope> THIS = this;
		
		if (func)
			m_LocalMap = func->local_map();
		
		size_t num_locals = m_LocalMap ? m_LocalMap->size() : 0;
		if (num_locals > 0) {
			THIS->m_Locals = gc_new<Array>();
			THIS->m_Locals->preallocate(num_locals);
			for (size_t i = 0; i < num_locals; ++i) {
				(*THIS->m_Locals)[i] = nil();
			}
		}
	}
	
	GC_ROOTS_IMPL(Scope) {
		GC_SUPER(ThinObject);

		GC_ROOT(m_Self);
		GC_ROOT(m_Function);
		GC_ROOT(m_LocalMap);
		GC_ROOT(m_Arguments);
		GC_ROOT(m_Locals);
		GC_ROOT(m_CallingScope);
	}
	
	bool Scope::has_local(Symbol name) {
		if (!m_LocalMap)
			return false;
		else
			return m_LocalMap->has_local(name);
	}
	
	Value Scope::get_local(Symbol name) {
		static Symbol self_symbol("self");
		if (name == self_symbol && m_Self)
			return m_Self;
			
		ASSERT(has_local(name));
		
		return m_Locals->get_by_index(local_map()->local(name));
	}
	
	Value Scope::set_local(Symbol name, const Value& val) {
		if (!m_LocalMap)
			m_LocalMap = gc_new<LocalMap>();
		if (!m_Locals)
			m_Locals = gc_new<Array>();
		
		uintx idx;
		
		if (!has_local(name)) {
			idx = local_map()->define_local(name);
		} else {
			idx = local_map()->local(name);
		}

		return m_Locals->set_by_index(idx, val);
	}
	
	Value Scope::self() const {
		if (m_Self)
			return m_Self;
		if (m_Function && m_Function->parent_scope())
			return m_Function->parent_scope()->self();
		return nil();
	}
	
	
	static Value scope_self(const Value& self, const Arguments& args) {
		Ptr<Scope> scope = object_cast<Scope>(self);
		ASSERT(scope);
		return scope->self();
	}

	static Value scope_arguments(const Value& self, const Arguments& args) {
		Ptr<Scope> scope = object_cast<Scope>(self);
		ASSERT(scope);
		return scope->arguments();
	}

	static Value scope_locals(const Value& self, const Arguments& args) {
		Ptr<Scope> scope = object_cast<Scope>(self);
		ASSERT(scope);
		return scope->locals();
	}

	Ptr<Object> scope_prototype() {
		static Ptr<Object> proto;
		if (proto) return proto;
		return proto;
	}

	void scope_prototype_init() {
		Ptr<Object> proto = scope_prototype();
		proto = malloc_new<Object>();
		proto->set_raw("name", gc_new<String>("Scope"));
		proto->set_raw("self", gc_new<Function>(scope_self));
		proto->set_raw("arguments", gc_new<Function>(scope_arguments));
		proto->set_raw("locals", gc_new<Function>(scope_locals));
	}
}
