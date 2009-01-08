#ifndef CODEGEN_H_J0VKJE70
#define CODEGEN_H_J0VKJE70

#include "SymbolTable.h"
#include "CompiledCode.h"
#include "Label.h"
#include "Scope.h"
#include "ASTNode.h"

#include <string>

namespace snow {
	
class Codegen {
public:
	virtual ~Codegen() {}
	
	void realize(const ast::Node&);
	void realize(const ast::Scope&);
	
	virtual CompiledCode compile() = 0;
	virtual Scope function_entry(int num_locals) = 0;
	virtual void function_return() = 0;
	virtual void set_argument(int index, const Scope::Local& src) = 0;
	virtual void set_argument(int index, const Scope::Temporary& src) = 0;
	virtual void set_argument(int index, const void* ptr) = 0;
	virtual void set_argument(int index, int immediate) = 0;
	virtual void get_argument(int index, const Scope::Local& dst) = 0;
	virtual void get_argument(int index, const Scope::Temporary& dst) = 0;
	virtual void set_return(const Scope::Local& src) = 0;
	virtual void set_return(const Scope::Temporary& src) = 0;
	virtual void call(const char* symbol) = 0;
	virtual void call(const char* symbol, const Scope::Local& retval) = 0;
	virtual void call(const char* symbol, const Scope::Temporary& retval) = 0;
	virtual void debug_break() = 0;
};
}

#endif /* end of include guard: CODEGEN_H_J0VKJE70 */
