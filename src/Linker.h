#ifndef LINKER_H_52Q2YBED
#define LINKER_H_52Q2YBED

#include <string>
#include "SymbolTable.h"

namespace snow {
	class CompiledCode;
	
class Linker {
public:
	struct Info {
		std::string symbol;
		int offset;
		int ref_size;
		bool relative;
		int relative_offset;
		
		Info(const Info& other) : symbol(other.symbol), offset(other.offset), ref_size(other.ref_size), relative(other.relative), relative_offset(other.relative_offset) {}
		Info(const std::string& sym, int offs, int ref_sz = 4, bool rel = false, int rel_offset = 0) : symbol(sym), offset(offs), ref_size(ref_sz), relative(rel), relative_offset(rel_offset) {}
	};
	
	static void register_symbols(const CompiledCode& code, SymbolTable& table);
	static void link(CompiledCode& code, const SymbolTable& table);
};
}

#endif /* end of include guard: LINKER_H_52Q2YBED */
