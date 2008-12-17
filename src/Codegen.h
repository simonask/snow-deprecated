#ifndef CODEGEN_H_J0VKJE70
#define CODEGEN_H_J0VKJE70

#include "SymbolTable.h"
#include "Label.h"

#include <string>

namespace snot {
class Codegen {
protected:
	SymbolTable& m_Table;
public:
	Codegen(SymbolTable& tbl) : m_Table(tbl) {}
	virtual ~Codegen() {}
	SymbolTable& table() const { return m_Table; }
	
	virtual void function_entry(int stacksize) = 0;
	virtual void function_return() = 0;
/*	virtual void set_function_argument(int index, const Operand&) = 0;
	virtual void get_function_argument(int index, const Operand&) = 0;*/
};
}

#endif /* end of include guard: CODEGEN_H_J0VKJE70 */
