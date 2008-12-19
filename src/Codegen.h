#ifndef CODEGEN_H_J0VKJE70
#define CODEGEN_H_J0VKJE70

#include "SymbolTable.h"
#include "CompiledCode.h"
#include "Label.h"
#include "Local.h"

#include <string>

namespace snot {
	
class Codegen {
public:
	virtual ~Codegen() {}
	
	virtual CompiledCode compile() = 0;
	virtual Local local(const char* name = 0) = 0;
	virtual void function_entry(int num_locals) = 0;
	virtual void function_return() = 0;
	virtual void set_argument(int index, const Local& src) = 0;
	virtual void set_argument(int index, const void* ptr) = 0;
	virtual void set_argument(int index, int immediate) = 0;
	virtual void get_argument(int index, const Local& dst) = 0;
	virtual void set_return(const Local& src) = 0;
	virtual void call(const char* symbol, const Local& retval) = 0;
};
}

#endif /* end of include guard: CODEGEN_H_J0VKJE70 */
