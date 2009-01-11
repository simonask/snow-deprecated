#ifndef CODEGEN_H_J0VKJE70
#define CODEGEN_H_J0VKJE70

#include "SymbolTable.h"
#include "CompiledCode.h"
#include "Label.h"
#include "Scope.h"
#include "ASTNode.h"
#include "StackFrame.h"

#include <string>

namespace snow {
	
class Codegen {
public:
	struct Local {
		int index;
		std::string name;
		Local(int index) : index(index), name("<anonymous local>") {}
		Local(int index, const std::string& name) : index(index), name(name) {}
		
		bool operator==(const Local& other) const { return index == other.index; }
		bool operator!=(const Local& other) const { return !(*this == other); }
	};
	
	virtual ~Codegen() {}
	
	// Create a Codegen for the current arch
	static RefPtr<Codegen> create();
	
	virtual RefPtr<CompiledCode> compile() = 0;
	virtual void function_entry(int num_locals) = 0;
	virtual void function_return() = 0;
	virtual void set_local(const Local& dst, const Local& src) = 0;
	virtual void set_local(const Local& dst, VALUE val) = 0;
	virtual void set_argument(int index, const Local& src) = 0;
	virtual void set_argument(int index, const Scope::Temporary& src) = 0;
	virtual void set_argument(int index, const void* ptr) = 0;
	virtual void set_argument(int index, int immediate) = 0;
	virtual void get_argument(int index, const Local& dst) = 0;
	virtual void get_argument(int index, const Scope::Temporary& dst) = 0;
	virtual void set_return(const Local& src) = 0;
	virtual void set_return(const Scope::Temporary& src) = 0;
	virtual void call(const char* symbol) = 0;
	virtual void call(const char* symbol, const Local& retval) = 0;
	virtual void call(const char* symbol, const Scope::Temporary& retval) = 0;
	virtual void debug_break() = 0;
};
}

#endif /* end of include guard: CODEGEN_H_J0VKJE70 */
