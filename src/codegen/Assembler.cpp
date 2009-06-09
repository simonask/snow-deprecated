#include "Assembler.h"
#include "base/Basic.h"

namespace snow {	
	Assembler::~Assembler() {
		clear();
	}

	Assembler::CodeBuffer& Assembler::buffer() {
		if (m_Buffer && m_Buffer->offset >= kCodeBufferSize) {
			m_FullBuffers.push_back(m_Buffer);
			m_Buffer = NULL;
		}
		if (!m_Buffer) {
			m_Buffer = new CodeBuffer;
		}
		ASSERT(m_Buffer);
		return *m_Buffer;
	}


	size_t Assembler::translate_offset(size_t internal_offset) const {
		ASSERT(internal_offset <= m_Length);

		auto subasm_iter = m_Subasms.begin();
		if (subasm_iter == m_Subasms.end())
			return internal_offset;

		size_t external_offset = internal_offset;
		while (subasm_iter != m_Subasms.end() && subasm_iter->first < internal_offset) {
			external_offset += subasm_iter->second->length();
			++subasm_iter;
		}

		return external_offset;
	}
	
	void Assembler::subasm(RefPtr<Assembler> m) {
		m_Subasms.push_back(SubassemblerInfo(offset(), m));
	}
	
	void Assembler::bind(const RefPtr<Label>& label) {
		label->bind(offset());
	}
	
	Linker::Symbol Assembler::define_symbol(const std::string& name) {
		return Linker::define_symbol(m_InternalSymbols, name, Linker::Symbol(offset()));
	}
	
	void Assembler::compile_to(CompiledCode& code, size_t start_offset) const {
		byte* target_buffer = code.code();
		
		
		// Copy machine code
		auto all_buffers = m_FullBuffers;
		all_buffers.push_back(m_Buffer);

		auto subasm_iter = m_Subasms.begin();
		auto buffer_iter = all_buffers.begin();
		size_t target_offset = 0;
		size_t buffer_start = 0;
		while (buffer_iter != all_buffers.end())
		{
			CodeBuffer& buffer = **buffer_iter;
			size_t buffer_end = buffer_start + buffer.offset;

			if (subasm_iter == m_Subasms.end() || subasm_iter->first >= buffer_end) {
				// Simple copy
				memcpy(&target_buffer[target_offset], buffer.data, buffer.offset);
				target_offset += buffer.offset;
				++buffer_iter;
			}
			else if (subasm_iter != m_Subasms.end() && subasm_iter->first < buffer_end) {
				// Interleave until subasm offset is > buffer_end
				size_t buffer_offset = buffer_start;

				while (buffer_offset < buffer_end) {
					if (buffer_offset < subasm_iter->first) {
						size_t buffer_len = subasm_iter->first - buffer_offset;
						memcpy(&target_buffer[target_offset], &buffer.data[buffer_offset], buffer_len);
						target_offset += buffer_len;
						buffer_offset += buffer_len;
					}
					// TODO: Make compile_to and length one call to avoid duplicate work
					subasm_iter->second->compile_to(code, target_offset);
					target_offset += subasm_iter->second->length();

					++subasm_iter;

					if (subasm_iter == m_Subasms.end() || subasm_iter->first > buffer_end) {
						size_t buffer_len = buffer_end - buffer_offset;
						memcpy(&target_buffer[target_offset], &buffer.data[buffer_offset], buffer_len);
						target_offset += buffer_len;
						buffer_offset += buffer_len;
						++buffer_iter;
						break;
					}
				}
			}

			buffer_start = buffer_end;
		}

		// Set label references
		for each (iter, m_UnboundLabelReferences) {
			if (!iter->label->bound())
				error("Reference to unbound label!");
			int reference_offset = start_offset + translate_offset(iter->offset);
			int offset = start_offset + translate_offset(iter->label->offset()) - (reference_offset + 4);
			byte* _offset = reinterpret_cast<byte*>(&offset);
			for (int i = 0; i < iter->size; ++i) {
				target_buffer[reference_offset + i] = _offset[i];
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
	
	CompiledCode* Assembler::compile() const {
		CompiledCode* code = gc_new<CompiledCode>(length());
		compile_to(*code, 0);
		return code;
	}
	
	void Assembler::clear() {
		delete m_Buffer;
		for each (iter, m_FullBuffers) {
			delete *iter;
		}
		m_InternalSymbols.clear();
		m_UnboundLabelReferences.clear();
		m_SymbolReferences.clear();
	}
}
