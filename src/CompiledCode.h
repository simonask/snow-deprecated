#ifndef COMPILEDCODE_H_1G9CD943
#define COMPILEDCODE_H_1G9CD943

#include <vector>
#include <string>
#include "Basic.h"
#include "SymbolTable.h"
#include "Linker.h"
#include "lib/Function.h"

namespace snow {
	class CompiledCode {
	private:
		byte* m_Code;
		int m_Size;
		std::vector<Linker::Info> m_SymbolReferences;
		SymbolTable m_SymbolTable;
		
		std::vector<RefPtr<CompiledCode>> m_Related;
	public:
		explicit CompiledCode(int size);
		virtual ~CompiledCode();
		
		inline int size() const { return m_Size; }
		inline byte* code() { return m_Code; }
		inline const byte* code() const { return m_Code; }
		inline FunctionPtr function() const { return (FunctionPtr)m_Code; }
		inline const SymbolTable& symbol_table() const { return m_SymbolTable; }
		inline std::vector<Linker::Info>& symbol_references() { return m_SymbolReferences; }
		
		void set_symbol(const std::string& name, int offset);
		void set_symbol_reference(const Linker::Info& info);
		void add_related(RefPtr<CompiledCode> rel) { m_Related.push_back(rel); }
		
		void export_symbols(SymbolTable& table) const;
		void link(const SymbolTable& table);
		void make_executable();
	};
}

#endif /* end of include guard: COMPILEDCODE_H_1G9CD943 */
