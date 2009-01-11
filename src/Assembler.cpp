#include "Assembler.h"
#include "Basic.h"
#include "Iterate.h"

namespace snow {	
	size_t Assembler::translate_offset(size_t internal_offset) const {
		size_t add = 0;
		for each (iter, m_SubAsms) {
			if (iter->first < internal_offset && iter->second.size() > 0) {
				for each (asm_iter, iter->second) {
					add += (*asm_iter)->length();
				}
			}
		}
		return internal_offset + add;
	}
	
	void Assembler::subasm(RefPtr<Assembler> m) {
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
		return snow::define_symbol(m_InternalSymbols, name, Symbol(offset()));
	}
	
	void Assembler::compile_to(CompiledCode& code, size_t start_offset) const {
		byte* buffer = code.code();
		
		if (!m_UnboundLabelReferences.empty()) {
			warn("Unbound label references exist!");
		}
		
		int len = length();
		
		// Copy machine code
		int target_i = start_offset;
		for (int i = 0; i < len; ++i, ++target_i) {
			auto subasm_iter = m_SubAsms.find(i);
			if (subasm_iter != m_SubAsms.end()) {
				for (auto iter = iterate(subasm_iter->second); iter; ++iter) {
					(*iter)->compile_to(code, target_i);
					target_i += (*iter)->length();
				}
			}
			buffer[target_i] = m_Code[i];
		}
		
		// Copy symbols.
		for each (table_iter, m_InternalSymbols) {
			code.set_symbol(table_iter->first, translate_offset(table_iter->second.offset()));
		}
		
		// Register symbol references
		for each (iter, m_SymbolReferences) {
			Linker::Info info(*iter);
			info.offset = translate_offset(info.offset);
			code.set_symbol_reference(info);
		}
	}
	
	RefPtr<CompiledCode> Assembler::compile() const {
		RefPtr<CompiledCode> code = new CompiledCode(length());
		compile_to(*code, 0);
		return code;
	}
	
	void Assembler::clear() {
		m_Code.clear();
		m_InternalSymbols.clear();
		m_UnboundLabelReferences.clear();
		m_SymbolReferences.clear();
	}
}