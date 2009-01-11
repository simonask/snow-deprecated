#ifndef SCOPE_H_IRS8EJUA
#define SCOPE_H_IRS8EJUA

#include <vector>

namespace snow {
	class Scope {
	public:
		struct Local {
			int index;
			const char* name;
			
			Local() : index(-1), name(NULL) {}
			Local(int idx, const char* name = NULL) : index(idx), name(name) {}
			bool operator==(const Local& other) const { return index == other.index; }
			bool operator!=(const Local& other) const { return index != other.index; }
		};
		
		class LocalList {
		private:
			std::unordered_map<std::string, Local> m_Locals;
			int m_NextIndex;
		public:
			LocalList() : m_NextIndex(0) {}
			void add(const std::string& name) {
				if (m_Locals.find(name) == m_Locals.end())
					m_Locals[name] = Local(m_NextIndex++, name.c_str());
			}
			int size() const { return m_NextIndex; }
			bool contains(const std::string& name) const {
				return m_Locals.find(name) != m_Locals.end();
			}
			const Local& get(const std::string& name) const {
				auto iter = m_Locals.find(name);
				if (iter == m_Locals.end())
					error("no such local: %s", name.c_str());
				return iter->second;
			}
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
