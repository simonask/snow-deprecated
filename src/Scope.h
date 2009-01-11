#ifndef SCOPE_H_IRS8EJUA
#define SCOPE_H_IRS8EJUA

#include <vector>

namespace snow {
	class Scope {
	public:
		class LocalList {
		private:
			std::unordered_map<std::string, int> m_Locals;
			int m_NextIndex;
		public:
			LocalList() : m_NextIndex(0) {}
			void add(const std::string& name) {
				if (m_Locals.find(name) == m_Locals.end())
					m_Locals[name] = m_NextIndex++;
			}
			int size() const { return m_NextIndex; }
			int index_for(const std::string& name) {
				auto iter = m_Locals.find(name);
				if (iter != m_Locals.end())
					return iter->second;
				else
					return -1;
			}
		};
		
		struct Local {
			int index;
			const char* name;
			
			Local(int idx, const char* name = NULL) : index(idx), name(name) {}
		};
		
		struct Temporary {
			Scope& scope;
			int index;
			Temporary(Scope& s, int idx) : scope(s), index(idx) {}
			Temporary(const Temporary& other) : scope(other.scope), index(other.index) {}
		};
	private:
		LocalList m_Locals;
		int m_NextTemporary;
		std::vector<Temporary> m_Temporaries;
	public:
		Scope() : m_NextTemporary(0) {}
		LocalList& locals() { return m_Locals; }
		Local local(const char* name = NULL) { return Local(0, name); }
		Temporary temp() { return Temporary(*this, m_NextTemporary++); }
	};
}

#endif /* end of include guard: SCOPE_H_IRS8EJUA */
