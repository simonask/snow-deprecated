#include "CompiledCode.h"
#include "gc/ExecutableAllocator.h"
#include "runtime/Function.h"
#include "Linker.h"
#include <sys/mman.h>

namespace snow {
	CompiledCode::CompiledCode(int size) : m_Size(size), m_LocalMap(NULL) {
		m_Code = new(kExecutable) byte[size];
	}
	
	CompiledCode::~CompiledCode() {
	}

	GC_ROOTS_IMPL(CompiledCode) {
		GC_ROOT(m_LocalMap);
		for each (iter, m_Related) {
			GC_ROOT(*iter.iterator());
		}
	}
	
	void CompiledCode::set_symbol(const std::string& symbol, int offset) {
		m_SymbolTable[symbol] = Linker::Symbol(offset);
	}
	
	void CompiledCode::set_symbol_reference(const Linker::Info& info) {
		m_SymbolReferences.push_back(info);
	}
	
	void CompiledCode::make_executable() {
		dynamic_cast<ExecutableAllocator&>(MemoryManager::allocator(kExecutable)).make_executable(m_Code);
		for each (iter, m_Related) {
			(*iter)->make_executable();
		}
	}
	
	void CompiledCode::export_symbols(Linker::SymbolTable& table) const {
		for each (iter, m_SymbolTable) {
			Linker::Symbol ext_symbol = iter->second.to_external(m_Code);
			Linker::define_symbol(table, iter->first, ext_symbol);
		}
		
		for each (iter, m_Related) {
			(*iter)->export_symbols(table);
		}
	}
	
	void CompiledCode::link(const Linker::SymbolTable& table) {
		Linker::link(*this, table);
		for each (iter, m_Related) {
			(*iter)->link(table);
		}
	}
	
	void CompiledCode::add_comment(size_t offset, const std::string& channel, const std::string& comment) {
		m_CommentChannels[channel][offset].push_back(comment);
	}
}
