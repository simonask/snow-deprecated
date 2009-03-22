#include "CompiledCode.h"
#include "ExecutableAllocator.h"
#include "lib/Function.h"
#include "Linker.h"
#include <sys/mman.h>

namespace snow {
	CompiledCode::CompiledCode(int size) : m_Size(size) {
		m_Code = new(kExecutable) byte[size];
	}
	
	CompiledCode::~CompiledCode() {
	}
	
	void CompiledCode::set_symbol(const std::string& symbol, int offset) {
		m_SymbolTable[symbol] = Symbol(offset);
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
	
	void CompiledCode::export_symbols(SymbolTable& table) const {
		for each (iter, m_SymbolTable) {
			Symbol ext_symbol = iter->second.to_external(m_Code);
			define_symbol(table, iter->first, ext_symbol);
		}
		
		for each (iter, m_Related) {
			(*iter)->export_symbols(table);
		}
	}
	
	void CompiledCode::link(const SymbolTable& table) {
		Linker::link(*this, table);
		for each (iter, m_Related) {
			(*iter)->link(table);
		}
	}
	
	void CompiledCode::add_comment(size_t offset, const std::string& channel, const std::string& comment) {
		m_CommentChannels[channel][offset].push_back(comment);
	}
}