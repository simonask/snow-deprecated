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
	
	void Assembler::bind(const RefPtr<Label>& label) {
		label->bind(offset());
	}
	
	Symbol Assembler::define_symbol(const std::string& name) {
		return snow::define_symbol(m_InternalSymbols, name, Symbol(offset()));
	}
	
	void Assembler::compile_to(CompiledCode& code, size_t start_offset) const {
		byte* buffer = code.code();
		
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
		
		// Set label references
		for each (iter, m_UnboundLabelReferences) {
			if (!iter->label->bound())
				error("Reference to unbound label!");
			int reference_offset = start_offset + translate_offset(iter->offset);
			int offset = start_offset + translate_offset(iter->label->offset()) - (reference_offset + 4);
			byte* _offset = reinterpret_cast<byte*>(&offset);
			for (int i = 0; i < iter->size; ++i) {
				buffer[reference_offset + i] = _offset[i];
			}
		}
		
		// Copy symbols.
		for each (table_iter, m_InternalSymbols) {
			code.set_symbol(table_iter->first, start_offset + translate_offset(table_iter->second.offset()));
		}
		
		// Register symbol references
		for each (iter, m_SymbolReferences) {
			Linker::Info info(*iter);
			info.offset = start_offset + translate_offset(info.offset);
			code.set_symbol_reference(info);
		}
		
		// Copy comments
		for each (iter, m_Comments) {
			for each (comment_iter, iter->second) {
				code.add_comment(translate_offset(iter->first), "Assembler", *comment_iter);
			}
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