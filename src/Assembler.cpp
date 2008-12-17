#include "Assembler.h"
#include "Basic.h"
#include <iostream>
using namespace std;

namespace snot {
	void Assembler::bind(Label& label) {
		label.bind(m_Code.size());
		
		auto iter = m_UnboundLabelReferences.begin();
		while (iter != m_UnboundLabelReferences.end()) {
			if (*iter->label == label) {
				int offset = label.offset() - (iter->offset + 4);
				byte* _offset = reinterpret_cast<byte*>(&offset);
				for (int i = 0; i < iter->size; ++i) {
					m_Code[iter->offset + i] = _offset[i];
				}
				
				iter = m_UnboundLabelReferences.erase(iter);
				if (iter == m_UnboundLabelReferences.end())
					break;
			}
			
			if (iter != m_UnboundLabelReferences.end())
				++iter;
		}
	}
	
	Symbol Assembler::define_symbol(const std::string& name) {
		return snot::define_symbol(m_InternalSymbols, name, Symbol(offset()));
	}
	
	CompiledCode Assembler::compile() {
		CompiledCode code(length());
		byte* buffer = code.code();
		
		// Copy machine code
		for (int i = 0; i < m_Code.size(); ++i) {
			buffer[i] = m_Code[i];
		}
		
		if (!m_UnboundLabelReferences.empty()) {
			cerr << "WARNING: Unbound label references exist!" << endl;
		}
		
		// Copy symbols.
		for (auto table_iter = m_InternalSymbols.begin(); table_iter != m_InternalSymbols.end(); ++table_iter) {
			code.set_symbol(table_iter->first, table_iter->second.offset());
		}
		
		// Register symbol references
		for (auto iter = m_SymbolReferences.begin(); iter != m_SymbolReferences.end(); ++iter) {
			code.set_symbol_reference(*iter);
		}
		
		m_Code.clear();
		m_InternalSymbols.clear();
		m_UnboundLabelReferences.clear();
		m_SymbolReferences.clear();
		
		return code;
	}
}