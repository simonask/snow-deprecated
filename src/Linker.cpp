#include "Linker.h"
#include "CompiledCode.h"
#include "Basic.h"

using namespace std;

namespace snot {
	void Linker::register_symbols(const CompiledCode& code, SymbolTable& table) {
		for (auto iter = code.symbol_table().begin(); iter != code.symbol_table().end(); ++iter) {
			Symbol ext_symbol = iter->second.to_external(code.code());
			define_symbol(table, iter->first, ext_symbol);
		}
	}
	
	void Linker::link(CompiledCode& code, const SymbolTable& table) {
		auto symrefs = code.symbol_references();
		byte* data = code.code();
		
		for (vector<Linker::Info>::iterator iter = symrefs.begin();;) {
			auto st_iter = table.find(iter->symbol);
			if (st_iter != table.end()) {
				Symbol symbol = st_iter->second;
				void* address = symbol.address();
				if (iter->relative) {
					void* rel = (void*)(((int64_t)address - ((int64_t)data + iter->offset)) + (int64_t)iter->relative_offset);
					address = rel;
				}
				
				byte* sym_data = reinterpret_cast<byte*>(&address);
				memcpy(&data[iter->offset], sym_data, iter->ref_size);
			} else {
				error("(linker) Unresolved symbol: `%s'!", iter->symbol.c_str());
			}
			
			iter = symrefs.erase(iter);
			if (iter == symrefs.end())
				break;
		}
	}
}