#include "CompiledCode.h"
#include "lib/IncrementalAlloc.h"
#include <iostream>
#include <sys/mman.h>
using namespace std;

namespace snot {
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
	}
}