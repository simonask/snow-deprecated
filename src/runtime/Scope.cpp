#include "runtime/Scope.h"
#include "runtime/Function.h"
#include "runtime/SnowString.h"
#include "runtime/Exception.h"

namespace snow {
	Scope::Scope(Function* func) : 
		ThinObject(scope_prototype()),
		m_Self(NULL),
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

	Scope::Scope(const Scope& other) :
		ThinObject(scope_prototype()),
		m_Self(other.m_Self),
		m_Function(other.m_Function),
		m_LocalMap(other.m_LocalMap),
		m_Arguments(other.m_Arguments),
		m_Locals(other.m_Locals),
		m_CallingScope(other.m_CallingScope)
	{
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
	
	bool Scope::has_local(VALUE name) {
		if (!m_LocalMap)
			return false;
		else
			return m_LocalMap->has_local(name);
	}
	
	VALUE Scope::get_local(VALUE name) {
		static VALUE self_symbol = symbol("self");
		if (name == self_symbol && m_Self)
			return m_Self;
			
		ASSERT(has_local(name));
		
		return m_Locals->get_by_index(local_map()->local(name));
	}
	
	VALUE Scope::set_local(VALUE name, VALUE val) {
		if (name == symbol("self")) {
			throw_exception(new String("Trying to set `self'!"));
		}
		
		if (!m_LocalMap)
			m_LocalMap = new LocalMap;
		if (!m_Locals)
			m_Locals = new Array;
		
		uint64_t idx;
		
		if (!has_local(name)) {
			idx = local_map()->define_local(name);
		} else {
			idx = local_map()->local(name);
		}

		return m_Locals->set_by_index(idx, val);
	}
	
	VALUE Scope::self() const {
		if (m_Self)
			return m_Self;
		if (m_Function && m_Function->parent_scope())
			return m_Function->parent_scope()->self();
		return nil();
	}
	
	
	static VALUE scope_self(VALUE self, uint64_t, VALUE*) {
		Scope* scope = object_cast<Scope>(self);
		ASSERT(scope);
		return scope->self();
	}

	static VALUE scope_arguments(VALUE self, uint64_t, VALUE*) {
		Scope* scope = object_cast<Scope>(self);
		ASSERT(scope);
		return scope->arguments();
	}

	static VALUE scope_locals(VALUE self, uint64_t, VALUE*) {
		Scope* scope = object_cast<Scope>(self);
		ASSERT(scope);
		return scope->locals();
	}

	Object* scope_prototype() {
		static Object* proto = NULL;
		if (proto) return proto;
		proto = new(kMalloc) Object;
		proto->set_raw_s("name", new String("Scope"));
		proto->set_raw_s("self", new Function(scope_self));
		proto->set_raw_s("arguments", new Function(scope_arguments));
		proto->set_raw_s("locals", new Function(scope_locals));
		return proto;
	}
}
