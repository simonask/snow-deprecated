#include "CompiledCode.h"
#include "lib/IncrementalAlloc.h"
#include <iostream>
#include <sys/mman.h>
using namespace std;

namespace snot {
	CompiledCode::CompiledCode(int size) : m_Size(size) {
		m_Code = (unsigned char*)incremental_alloc(size);
	}
	
	CompiledCode::~CompiledCode() {
		incremental_free(m_Code);
	}
	
	void CompiledCode::set_symbol(const std::string& symbol, int offset) {
		m_SymbolTable[symbol] = Symbol(offset);
	}
	
	void CompiledCode::set_symbol_reference(const std::string& symbol, int offset, int size) {
		m_SymbolReferences.push_back(Linker::Info(symbol, offset, size));
	}
	
	void CompiledCode::resolve_symbol_references(const SymbolTable& table) {
		for (vector<Linker::Info>::iterator iter = m_SymbolReferences.begin();;) {
			SymbolTable::const_iterator st_iter = table.find(iter->symbol);
			if (st_iter != table.end()) {
				Symbol symbol = st_iter->second;
				void* data = symbol.address();
				unsigned char* sym_data = reinterpret_cast<unsigned char*>(&data);
				memcpy(&m_Code[iter->offset], sym_data, iter->ref_size);
			} else {
				cerr << "LINKING ERROR: Unresolved symbol: `" << iter->symbol << "'!" << endl;
			}
			
			iter = m_SymbolReferences.erase(iter);
			if (iter == m_SymbolReferences.end())
				break;
		}
	}
	
	void CompiledCode::make_executable() {
		incremental_make_executable(m_Code);
	}
}