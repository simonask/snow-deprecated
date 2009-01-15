#ifndef SCOPE_H_IRS8EJUA
#define SCOPE_H_IRS8EJUA

#include <vector>

namespace snow {
	class Scope {
	public:
		struct Local {
			Scope* scope;
			int index;
			const char* name;
			
			Local() : scope(NULL), index(-1), name(NULL) {}
			Local(Scope* scope, int idx, const char* name = NULL) : scope(scope), index(idx), name(name) {}
			bool operator==(const Local& other) const { return index == other.index; }
			bool operator!=(const Local& other) const { return index != other.index; }
		};
		
		typedef std::unordered_map<std::string, Local> LocalList;
		
		struct Temporary {
			Scope& scope;
			int index;
			Temporary(Scope& s, int idx) : scope(s), index(idx) {}
			Temporary(const Temporary& other) : scope(other.scope), index(other.index) {}
		};
	private:
		LocalList m_Locals;
		int m_NextTemporary;
		int m_NextLocal;
		std::vector<Temporary> m_Temporaries;
	public:
		Scope() : m_NextTemporary(0), m_NextLocal(0) {}
		LocalList& locals() { return m_Locals; }
		void add_local(const std::string& name) {
			if (m_Locals.find(name) == m_Locals.end())
				m_Locals[name] = Local(this, m_NextLocal++, name.c_str());
		}
		int num_locals() const { return m_NextLocal; }
		bool contains_local(const std::string& name) const { return m_Locals.find(name) != m_Locals.end(); }
		const Local& get_local(const std::string& name) const {
			auto iter = m_Locals.find(name);
			if (iter == m_Locals.end())
				error("no such local: `%s' in scope: 0x%llx", name.c_str(), this);
			return iter->second;
		}
		
		Temporary temp() { return Temporary(*this, m_NextTemporary++); }
	};
}

#endif /* end of include guard: SCOPE_H_IRS8EJUA */
