#ifndef LOCALMAP_H_QGZ58WXV
#define LOCALMAP_H_QGZ58WXV

#include <unordered_map>
#include "Basic.h"
#include "IGarbage.h"

namespace snow {
	/*
		A LocalMap is a mapping from the name of a local variable in a scope
		to the index of the same local variable in that scope's array of
		locals.
		
		This is part of the optimization that allows locals to have constant
		lookup time, because we know the contents of the LocalMap at compile-time.

		The map can be changed at runtime as well, though.
		This happens in global scope when external source files are included.
	*/
	class LocalMap : public IGarbage {
	private:
		std::unordered_map<std::string, uint64_t> m_Map;
		uint64_t m_NumLocals;

		GC_ROOTS {}
	public:
		LocalMap() : m_NumLocals(0) {}
		LocalMap(const LocalMap& other) : m_Map(other.m_Map), m_NumLocals(other.m_NumLocals) {}
		
		bool has_local(const std::string& name) const { return m_Map.find(name) != m_Map.end(); }
		uint64_t local(const std::string& name) const;
		uint64_t define_local(const std::string& name);
		
		uint64_t size() const { return m_NumLocals; }
		const std::unordered_map<std::string, uint64_t>& map() const { return m_Map; }
	};
}

#endif /* end of include guard: LOCALMAP_H_QGZ58WXV */
