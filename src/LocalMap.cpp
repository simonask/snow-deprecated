#include "LocalMap.h"

namespace snow {
	uint64_t LocalMap::local(const std::string& name) const {
		auto iter = m_Map.find(name);
		if (iter == m_Map.end()) {
			error("Local `%s' doesn't exist in this LocalMap.", name.c_str());
			TRAP();
		}
		return iter->second;
	}
	
	uint64_t LocalMap::define_local(const std::string& name) {
		auto iter = m_Map.find(name);
		if (iter == m_Map.end()) {
			auto l = m_NumLocals++;
			m_Map[name] = l;
			return l;
		} else {
			warn("Local `%s' already exists in this LocalMap.", name.c_str());
			return iter->second;
		}
	}
}