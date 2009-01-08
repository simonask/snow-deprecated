#ifndef SCOPE_H_IRS8EJUA
#define SCOPE_H_IRS8EJUA

#include <vector>

namespace snow {
	class Scope {
	public:
		struct Local {
			Scope& scope;
			int index;
			const char* name;
			
			Local(Scope& s, int idx, const char* name = NULL) : scope(s), index(idx), name(name) {}
			Local(const Local& other) : scope(other.scope), index(other.index) {}
		};
		
		struct Temporary {
			Scope& scope;
			int index;
			Temporary(Scope& s, int idx) : scope(s), index(idx) {}
			Temporary(const Temporary& other) : scope(other.scope), index(other.index) {}
		};
	private:
		int m_NextLocal;
		std::vector<Local> m_Locals;
		int m_NextTemporary;
		std::vector<Temporary> m_Temporaries;
	public:
		Scope() : m_NextLocal(0), m_NextTemporary(0) {}
		
		Local local(const char* name = NULL) { return Local(*this, m_NextLocal++, name); }
		Temporary temp() { return Temporary(*this, m_NextTemporary++); }
	};
}

#endif /* end of include guard: SCOPE_H_IRS8EJUA */
