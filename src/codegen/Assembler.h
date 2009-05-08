#ifndef ASSEMBLER_H_6M390CE5
#define ASSEMBLER_H_6M390CE5

#include "Label.h"
#include "Linker.h"
#include "CompiledCode.h"
#include "base/Basic.h"
#include <list>

namespace snow {
	class Assembler {
	private:
		enum { kCodeBufferSize = 1024 };

		typedef std::pair<size_t, RefPtr<Assembler>> SubassemblerInfo;

		struct CodeBuffer {
			byte* data;
			size_t offset;
			CodeBuffer() : data(NULL), offset(0) { data = new(kMalloc) byte[kCodeBufferSize]; }
			~CodeBuffer() { delete[] data; }
		};
		CodeBuffer* m_Buffer;
		std::list<CodeBuffer*> m_FullBuffers;
		std::list<SubassemblerInfo> m_Subasms;
		size_t m_Length;

		CompiledCode::CommentThread m_Comments;
		
		Assembler(const Assembler&) {}
		CodeBuffer& buffer();
	protected:
		void emit(byte code);
		
		Linker::SymbolTable m_InternalSymbols;
		
		struct UnboundLabelReference {
			RefPtr<Label> label;
			int offset;
			int size;
			bool absolute;
			
			UnboundLabelReference(RefPtr<Label> l, int offs, int sz = 4, bool abs = false) : label(l), offset(offs), size(sz), absolute(abs) {}
		};
		std::vector<UnboundLabelReference> m_UnboundLabelReferences;
		
		std::vector<Linker::Info> m_SymbolReferences;
		
		size_t translate_offset(size_t internal_offset) const;
		
		void compile_to(CompiledCode& code, size_t start_offset = 0) const;
	public:
		Assembler() : m_Buffer(NULL), m_Length(0) {}
		virtual ~Assembler();
		
		void bind(const RefPtr<Label>& label);
		inline size_t offset() const { return m_Length; }
		inline size_t length() const { return translate_offset(offset()); }
		CompiledCode* compile() const;
		void clear();
		Linker::Symbol define_symbol(const std::string& name);
		
		void subasm(RefPtr<Assembler>);
		
		void comment(const std::string& comm) { m_Comments[offset()].push_back(comm); }
	};

	inline void Assembler::emit(byte code) {
		CodeBuffer& buf = buffer();
		buf.data[buf.offset] = code;
		++buf.offset;
		++m_Length;
	}
}

#endif /* end of include guard: ASSEMBLER_H_6M390CE5 */
