#include "Linker.h"
#include "CompiledCode.h"
#include <iostream>

using namespace std;

namespace snot {
	void Linker::register_symbols(const CompiledCode& code, SymbolTable& table) {
		for (SymbolTable::const_iterator iter = code.symbol_table().begin(); iter != code.symbol_table().end(); ++iter) {
			Symbol ext_symbol = iter->second.to_external(code.code());
			define_symbol(table, iter->first, ext_symbol);
		}
	}
	
	void Linker::link(CompiledCode& code, const SymbolTable& table) {
		code.resolve_symbol_references(table);
	}
}