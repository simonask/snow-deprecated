#ifndef LINKER_H_52Q2YBED
#define LINKER_H_52Q2YBED

#include <string>
#include "CompiledCode.h"
#include "SymbolTable.h"

namespace snot {
class Linker {
public:
	struct Info {
		std::string symbol;
		int offset;
		int ref_size;
		Info(const std::string& sym, int offs, int ref_size = 4) : symbol(sym), offset(offs), ref_size(ref_size) {}
	};
	
	static void register_symbols(const CompiledCode& code, SymbolTable& table);
	static void link(CompiledCode& code, const SymbolTable& table);
};
}

#endif /* end of include guard: LINKER_H_52Q2YBED */
