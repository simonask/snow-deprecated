#ifndef LINKER_H_52Q2YBED
#define LINKER_H_52Q2YBED

#include <string>
#include "SymbolTable.h"

namespace snot {
	class CompiledCode;
	
class Linker {
public:
	struct Info {
		std::string symbol;
		int offset;
		int ref_size;
		bool relative;
		int relative_offset;
		
		Info(const std::string& sym, int offs, int ref_size = 4, bool rel = false, int rel_offset = 0) : symbol(sym), offset(offs), ref_size(ref_size), relative(rel), relative_offset(rel_offset) {}
	};
	
	static void register_symbols(const CompiledCode& code, SymbolTable& table);
	static void link(CompiledCode& code, const SymbolTable& table);
};
}

#endif /* end of include guard: LINKER_H_52Q2YBED */
