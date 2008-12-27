#ifndef ASSEMBLER_H_6M390CE5
#define ASSEMBLER_H_6M390CE5

#include "Label.h"
#include "Linker.h"
#include "SymbolTable.h"
#include "CompiledCode.h"
#include "Basic.h"
#include <vector>

namespace snot {
	class Assembler {
	private:
		std::vector<byte> m_Code;
		std::map<size_t, std::vector<Assembler*>> m_SubAsms;
	protected:
		inline void emit(byte code) { m_Code.push_back(code); }
		
		SymbolTable m_InternalSymbols;
		
		struct UnboundLabelReference {
			const Label* label;
			int offset;
			int size;
			bool absolute;
			
			UnboundLabelReference(const Label& l, int offs, int sz = 4, bool abs = false) : label(&l), offset(offs), size(sz), absolute(abs) {}
		};
		std::vector<UnboundLabelReference> m_UnboundLabelReferences;
		
		std::vector<Linker::Info> m_SymbolReferences;
		
		size_t translate_offset(size_t internal_offset) const;
		
		void compile_to(CompiledCode& code, size_t start_offset = 0);
	public:
		virtual ~Assembler() {}
		
		void bind(Label& label);
		inline size_t offset() const { return m_Code.size(); }
		inline size_t length() const { return translate_offset(offset()); }
		CompiledCode compile();
		void clear();
		Symbol define_symbol(const std::string& name);
		
		void subasm(Assembler*);
	};
}

#endif /* end of include guard: ASSEMBLER_H_6M390CE5 */
