#include "LocalMap.h"
#include "lib/Runtime.h"
#include "lib/SnowString.h"
#include "lib/Exception.h"

namespace snow {
	uint64_t LocalMap::local(VALUE name) const {
		auto iter = m_Map.find(name);
		if (iter == m_Map.end()) {
			throw_exception(new String("Local `%' doesn't exist in this LocalMap.", name));
		}
		return iter->second;
	}
	
	uint64_t LocalMap::define_local(VALUE name) {
		auto iter = m_Map.find(name);
		if (iter == m_Map.end()) {
			auto l = m_NumLocals++;
			m_Map[name] = l;
			return l;
		} else {
			warn("Local `%s' already exists in this LocalMap.", value_to_string(name));
			return iter->second;
		}
	}
}
