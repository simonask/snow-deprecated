#ifndef SCOPE_H_KJ3A0580
#define SCOPE_H_KJ3A0580

#include "lib/ThinObject.h"
#include "lib/Array.h"

namespace snow {
	class Function;
	
	Handle<Object>& scope_prototype();
	
	class Scope : public ThinObject {
	public:
		typedef std::unordered_map<std::string, uint64_t> LocalMap;
	private:
		ValueHandle m_Self;
		LocalMap m_LocalMap;
		Handle<Function> m_Function;
		Handle<Array> m_Arguments;
		Handle<Array> m_Locals;
		Handle<Scope> m_CallingScope;
	public:
		explicit Scope(const Handle<Function>& func = NULL);
		virtual ~Scope() {}
		
		const ValueHandle& self() const { return m_Self; }
		void set_self(const ValueHandle& self) { m_Self = self; }
		
		const Handle<Function>& function() const { return m_Function; }
		void set_function(const Handle<Function>& func) { m_Function = func; }
		
		const Handle<Array>& locals() const { return m_Locals; }
		Handle<Array> locals() { return m_Locals; }
		const LocalMap& local_map() const { return m_LocalMap; }
		bool has_local(const std::string& name) const;
		VALUE get_local(const std::string& name) const;
		VALUE set_local(const std::string& name, const ValueHandle& value);
		
		const Handle<Array>& arguments() const { return m_Arguments; }
		void set_arguments(const Handle<Array>& args) { m_Arguments = args; }
		
		const Handle<Scope>& calling_scope() const { return m_CallingScope; }
		void set_calling_scope(const Handle<Scope>& scope) { m_CallingScope = scope; }
	};
}

#endif /* end of include guard: SCOPE_H_KJ3A0580 */
