#include "Assembler.h"
#include "Basic.h"
#include "Iterate.h"
#include <iostream>
using namespace std;

namespace snot {
	Assembler::~Assembler() {
		for (auto iter = iterate(m_SubAsms); iter; ++iter) {
			for (auto asm_iter = iterate(iter->second); asm_iter; ++asm_iter) {
				delete *asm_iter;
			}
		}
	}
	
	size_t Assembler::translate_offset(size_t internal_offset) const {
		size_t add = 0;
		for (auto iter = iterate(m_SubAsms); iter; ++iter) {
			if (iter->first < internal_offset && iter->second.size() > 0) {
				for (auto asm_iter = iterate(iter->second); asm_iter; ++asm_iter) {
					add += (*asm_iter)->length();
				}
			}
		}
		return internal_offset + add;
	}
	
	void Assembler::subasm(Assembler* m) {
		m_SubAsms[offset()].push_back(m);
	}
	
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
	
	void Assembler::compile_to(CompiledCode& code, size_t start_offset) {
		byte* buffer = code.code();
		
		if (!m_UnboundLabelReferences.empty()) {
			cerr << "WARNING: Unbound label references exist!" << endl;
		}
		
		int len = length();
		
		// Copy machine code
		int target_i = start_offset;
		for (int i = 0; i < len; ++i, ++target_i) {
			if (m_SubAsms.find(i) != m_SubAsms.end()) {
				for (auto iter = iterate(m_SubAsms[i]); iter; ++iter) {
					(*iter)->compile_to(code, target_i);
					target_i += (*iter)->length();
				}
			}
			buffer[target_i] = m_Code[i];
		}
		
		// Copy symbols.
		for (auto table_iter = iterate(m_InternalSymbols); table_iter; ++table_iter) {
			code.set_symbol(table_iter->first, translate_offset(table_iter->second.offset()));
		}
		
		// Register symbol references
		for (auto iter = iterate(m_SymbolReferences); iter; ++iter) {
			Linker::Info info(*iter);
			info.offset = translate_offset(info.offset);
			code.set_symbol_reference(info);
		}
	}
	
	CompiledCode Assembler::compile() {
		CompiledCode code(length());
		compile_to(code, 0);
		return code;
	}
	
	void Assembler::clear() {
		m_Code.clear();
		m_InternalSymbols.clear();
		m_UnboundLabelReferences.clear();
		m_SymbolReferences.clear();
	}
}