#ifndef SYMBOLTABLE_H_QFD1ZLDG
#define SYMBOLTABLE_H_QFD1ZLDG

#include <tr1/unordered_map>
#include <string>

namespace snot {

class Symbol {
private:
	bool m_External;
	union {
		int32_t m_Offset;
		void* m_Address;
	};
public:
	Symbol() {}
	explicit Symbol(int32_t offset) : m_External(false) { m_Offset = offset; }
	explicit Symbol(void* address) : m_External(true) { m_Address = address; }
	bool external() const { return m_External; }
	int32_t offset() const { return m_Offset; }
	void* address() const { return m_Address; }
};

typedef std::tr1::unordered_map<std::string, Symbol> SymbolTable;

Symbol define_symbol(SymbolTable& table, const std::string& name, const Symbol& symb);

}

#endif /* end of include guard: SYMBOLTABLE_H_QFD1ZLDG */
