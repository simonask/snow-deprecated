#ifndef ASSEMBLER_H_6M390CE5
#define ASSEMBLER_H_6M390CE5

#include "Label.h"
#include "SymbolTable.h"
#include "CompiledCode.h"
#include <vector>

namespace snot {
	class Assembler {
	private:
		std::vector<unsigned char> m_Code;
	protected:
		inline void emit(unsigned char code) { m_Code.push_back(code); }
		
		SymbolTable m_InternalSymbols;
		
		struct UnboundLabelReference {
			const Label* label;
			int offset;
			int size;
			bool absolute;
			
			UnboundLabelReference(const Label& l, int offs, int sz = 4, bool abs = false) : label(&l), offset(offs), size(sz), absolute(abs) {}
		};
		std::vector<UnboundLabelReference> m_UnboundLabelReferences;
		
		struct UnboundExternalSymbolReference {
			std::string name;
			int offset;
			int size;
			
			UnboundExternalSymbolReference(const std::string n, int offs, int sz = 4) : name(n), offset(offs), size(sz) {}
		};
		std::vector<UnboundExternalSymbolReference> m_UnboundExternalSymbolReferences;
	public:
		virtual ~Assembler() {}
		
		void bind(Label& label);
		inline size_t offset() const { return m_Code.size(); }
		inline size_t length() const { return m_Code.size(); }
		CompiledCode compile();
		Symbol define_symbol(const std::string& name);
	};
}

#endif /* end of include guard: ASSEMBLER_H_6M390CE5 */
