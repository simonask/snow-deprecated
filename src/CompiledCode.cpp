#include "CompiledCode.h"
#include "lib/IncrementalAlloc.h"
#include "Linker.h"
#include <sys/mman.h>

namespace snow {
	CompiledCode::CompiledCode(int size) : m_Size(size) {
		m_Code = (byte*)incremental_alloc(size);
	}
	
	CompiledCode::~CompiledCode() {
		incremental_free(m_Code);
	}
	
	void CompiledCode::set_symbol(const std::string& symbol, int offset) {
		m_SymbolTable[symbol] = Symbol(offset);
	}
	
	void CompiledCode::set_symbol_reference(const Linker::Info& info) {
		m_SymbolReferences.push_back(info);
	}
	
	void CompiledCode::make_executable() {
		incremental_make_executable(m_Code);
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