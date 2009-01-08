#ifndef COMPILEDCODE_H_1G9CD943
#define COMPILEDCODE_H_1G9CD943

#include <vector>
#include <string>
#include "Basic.h"
#include "SymbolTable.h"
#include "Linker.h"

namespace snow {
	class CompiledCode {
	private:
		byte* m_Code;
		int m_Size;
		std::vector<Linker::Info> m_SymbolReferences;
		SymbolTable m_SymbolTable;
	public:
		explicit CompiledCode(int size);
		virtual ~CompiledCode();
		
		inline int size() const { return m_Size; }
		inline byte* code() { return m_Code; }
		inline const byte* code() const { return m_Code; }
		inline const SymbolTable& symbol_table() const { return m_SymbolTable; }
		inline std::vector<Linker::Info>& symbol_references() { return m_SymbolReferences; }
		
		void set_symbol(const std::string& name, int offset);
		void set_symbol_reference(const Linker::Info& info);
		
		void make_executable();
	};
}

#endif /* end of include guard: COMPILEDCODE_H_1G9CD943 */
