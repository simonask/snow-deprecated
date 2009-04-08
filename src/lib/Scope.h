#ifndef SCOPE_H_KJ3A0580
#define SCOPE_H_KJ3A0580

#include "lib/ThinObject.h"
#include "lib/Array.h"
#include "lib/Function.h"

namespace snow {
	Handle<Object>& scope_prototype();
	
	class Scope : public ThinObject {
	private:		
		GC_ROOTS;

		// These could be handles, but a Handle is 4 times as big as a pointer.
		VALUE m_Self;
		Function* m_Function;
		LocalMap* m_LocalMap;
		Array* m_Arguments;
		Array* m_Locals;
		Scope* m_CallingScope;
	public:
		explicit Scope(Function* func = NULL);
		Scope(const Scope& other); 
		virtual ~Scope() {}
		IObject* copy() const { return new Scope(*this); }
		
		VALUE self() const;
		void set_self(VALUE self) { m_Self = self; }
		
		Handle<Function> function() const { return m_Function; }
		
		Handle<Array> locals() const { return m_Locals; }
		Handle<LocalMap> local_map() const { return m_LocalMap; }
		bool has_local(VALUE name);
		VALUE get_local(VALUE name);
		VALUE set_local(VALUE name, const ValueHandle& value);
		VALUE get_local_by_string(const char* s) { return get_local(symbol(s)); }
		VALUE set_local_by_string(const char* s, VALUE val) { return set_local(symbol(s), val); }
		
		Handle<Array> arguments() const { return m_Arguments; }
		void set_arguments(const Handle<Array>& args) { m_Arguments = args; }
		
		Handle<Scope> calling_scope() const { return m_CallingScope; }
		void set_calling_scope(const Handle<Scope>& scope) { m_CallingScope = scope; }
	};
}

#endif /* end of include guard: SCOPE_H_KJ3A0580 */
