#ifndef LINKER_H_52Q2YBED
#define LINKER_H_52Q2YBED

#include "Basic.h"
#include <string>
#include <unordered_map>

namespace snow {
	class CompiledCode;
	
class Linker {
public:
	class Symbol {
	private:
		bool m_External;
		union {
			int32_t m_Offset;
			void* m_Address;
		};
	public:
		Symbol() {}
		explicit Symbol(int32_t offset) : m_External(false) { m_Offset = offset; }
		Symbol(void* address) : m_External(true) { m_Address = address; }
		bool external() const { return m_External; }
		int32_t offset() const { return m_Offset; }
		void* address() const { return m_Address; }
		
		Symbol to_external(const void* base) const { return Symbol(&((byte*)base)[m_Offset]); }
		
		bool operator==(const Symbol& other) const { return this == &other || (m_External == other.m_External && m_Address == other.m_Address); }
		bool operator!=(const Symbol& other) const { return !(*this == other); }
	};

	typedef std::unordered_map<std::string, Symbol> SymbolTable;

	struct Info {
		std::string symbol;
		int offset;
		int ref_size;
		bool relative;
		int relative_offset;
		
		Info(const Info& other) : symbol(other.symbol), offset(other.offset), ref_size(other.ref_size), relative(other.relative), relative_offset(other.relative_offset) {}
		Info(const std::string& sym, int offs, int ref_sz = 4, bool rel = false, int rel_offset = 0) : symbol(sym), offset(offs), ref_size(ref_sz), relative(rel), relative_offset(rel_offset) {}
	};
	
	static void link(CompiledCode& code, const SymbolTable& table);

	static Symbol define_symbol(SymbolTable& table, const std::string& name, const Symbol& symb);
};
}

#endif /* end of include guard: LINKER_H_52Q2YBED */
