#ifndef COMPILEDCODE_H_1G9CD943
#define COMPILEDCODE_H_1G9CD943

#include <vector>
#include <string>
#include "Basic.h"
#include "SymbolTable.h"
#include "Linker.h"
#include "CompiledCode.h"
#include "LocalMap.h"

namespace snow {
	class CompiledCode {
	public:
		typedef std::unordered_map<size_t, std::vector<std::string>> CommentThread;
		typedef std::unordered_map<std::string, CommentThread> CommentChannels;
	private:
		byte* m_Code;
		int m_Size;
		Handle<LocalMap> m_LocalMap;
		std::vector<Linker::Info> m_SymbolReferences;
		SymbolTable m_SymbolTable;
		
		std::vector<Handle<CompiledCode>> m_Related;
		
		CommentChannels m_CommentChannels;
	public:
		explicit CompiledCode(int size);
		virtual ~CompiledCode();
		
		inline int size() const { return m_Size; }
		inline byte* code() { return m_Code; }
		inline const byte* code() const { return m_Code; }
		inline FunctionPtr function_pointer() const { return (FunctionPtr)m_Code; }
		inline const SymbolTable& symbol_table() const { return m_SymbolTable; }
		inline std::vector<Linker::Info>& symbol_references() { return m_SymbolReferences; }
		
		void set_symbol(const std::string& name, int offset);
		void set_symbol_reference(const Linker::Info& info);
		void add_related(Handle<CompiledCode> rel) { m_Related.push_back(rel); }
		
		void set_local_map(const Handle<LocalMap>& map) { m_LocalMap = map; }
		const Handle<LocalMap>& local_map() const { return m_LocalMap; }
		
		void export_symbols(SymbolTable& table) const;
		void link(const SymbolTable& table);
		void make_executable();
		
		void add_comment(size_t offset, const std::string& channel, const std::string& comment);
		const CommentChannels& comment_channels() const { return m_CommentChannels; }
	};
}

#endif /* end of include guard: COMPILEDCODE_H_1G9CD943 */
