#include "LocalMap.h"
#include "runtime/Runtime.h"
#include "runtime/SnowString.h"
#include "runtime/Exception.h"

namespace snow {
	uintx LocalMap::local(VALUE name) const {
		auto iter = m_Map.find(name);
		if (iter == m_Map.end()) {
			throw_exception(gc_new<String>("Local `%' doesn't exist in this LocalMap.", name));
		}
		return iter->second;
	}
	
	uintx LocalMap::define_local(VALUE name) {
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
