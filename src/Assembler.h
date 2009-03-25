#ifndef ASSEMBLER_H_6M390CE5
#define ASSEMBLER_H_6M390CE5

#include "Label.h"
#include "Linker.h"
#include "SymbolTable.h"
#include "CompiledCode.h"
#include "Basic.h"
#include <vector>
#include <unordered_map>

namespace snow {
	class Assembler {
	private:
		std::vector<byte> m_Code;
		std::unordered_map<size_t, std::vector<RefPtr<Assembler>>> m_SubAsms;

		CompiledCode::CommentThread m_Comments;
		
		Assembler(const Assembler&) {}
	protected:
		inline void emit(byte code) { m_Code.push_back(code); }
		
		SymbolTable m_InternalSymbols;
		
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
		Assembler() {}
		virtual ~Assembler() {}
		
		void bind(const RefPtr<Label>& label);
		inline size_t offset() const { return m_Code.size(); }
		inline size_t length() const { return translate_offset(offset()); }
		Handle<CompiledCode> compile() const;
		void clear();
		Symbol define_symbol(const std::string& name);
		
		void subasm(RefPtr<Assembler>);
		
		void comment(const std::string& comm) { m_Comments[offset()].push_back(comm); }
	};
}

#endif /* end of include guard: ASSEMBLER_H_6M390CE5 */
