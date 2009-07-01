#ifndef SCOPE_H_KJ3A0580
#define SCOPE_H_KJ3A0580

#include "runtime/ThinObject.h"
#include "runtime/Array.h"
#include "runtime/Function.h"

namespace snow {
	Ptr<Object> scope_prototype();
	void scope_prototype_init();
	
	class Scope : public ThinObject {
	private:		
		GC_ROOTS;

		Value m_Self;
		Ptr<Function> m_Function;
		Ptr<LocalMap> m_LocalMap;
		Ptr<Array> m_Arguments;
		Ptr<Array> m_Locals;
		Ptr<Scope> m_CallingScope;
		bool m_GCLock;
		
		bool gc_try_lock() { bool v = !m_GCLock; if (v) { m_GCLock = true; } return v; }
		void gc_unlock() { m_GCLock = false; }
	public:
		explicit Scope(const Ptr<Function>& func = NULL);
		void initialize(const Ptr<Function>& func = NULL);
		Scope(const Scope& other); 
		void initialize(const Scope& other) {}
		virtual ~Scope() {}

		Ptr<IObject> copy() const { return gc_new<Scope>(*this); }
		
		Value self() const;
		void set_self(const Value& self) { m_Self = self; }
		
		Ptr<Function> function() const { return m_Function; }
		
		Ptr<Array> locals() const { return m_Locals; }
		Ptr<LocalMap> local_map() const { return m_LocalMap; }
		bool has_local(Symbol name);
		Value get_local(Symbol name);
		Value set_local(Symbol name, const Value& value);
		
		Ptr<Array> arguments() const { return m_Arguments; }
		void set_arguments(Ptr<Array> args) { m_Arguments = args; }
		
		Ptr<Scope> calling_scope() const { return m_CallingScope; }
		void set_calling_scope(Scope* scope) { m_CallingScope = scope; }
	};
	
	inline Scope::Scope(const Ptr<Function>& func) : 
		ThinObject(scope_prototype()),
		m_Function(func)
	{}
	
	inline Scope::Scope(const Scope& other) :
		ThinObject(scope_prototype()),
		m_Self(other.m_Self),
		m_Function(other.m_Function),
		m_LocalMap(other.m_LocalMap),
		m_Arguments(other.m_Arguments),
		m_Locals(other.m_Locals),
		m_CallingScope(other.m_CallingScope)
	{}
}

#endif /* end of include guard: SCOPE_H_KJ3A0580 */
