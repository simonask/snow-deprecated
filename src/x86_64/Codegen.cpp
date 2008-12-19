#include "Codegen.h"
#include <stdexcept>

#define __ m_Assembler.

namespace snot {
namespace x86_64 {
	Address Codegen::addr_for_local(const Local& local) {
		return Address(rbp, (local.index() + 1) * -8, true);
	}
	
	const Register& Codegen::reg_for_arg(int index) {
		static const Register* regs[] = { &rdi, &rsi, &rdx, &rcx, &r8, &r9 };
		return *regs[index];
	}
	
	CompiledCode Codegen::compile() {
		return __ compile();
	}
	
	Local Codegen::local(const char* name) {
		static int n = 0;
		return Local(n++, name);
	}
	
	void Codegen::function_entry(int num_locals) {
		int stack_size = 8 * num_locals;
		__ enter(stack_size);
	}
	
	void Codegen::function_return() {
		__ leave();
		__ ret();
	}
	
	void Codegen::set_argument(int index, const Local& src) {
		__ mov(addr_for_local(src), reg_for_arg(index));
	}
	
	void Codegen::set_argument(int index, const void* ptr) {
		__ mov((const char*)ptr, reg_for_arg(index));
	}
	
	void Codegen::set_argument(int index, int immediate) {
		__ mov(immediate, reg_for_arg(index));
	}
	
	void Codegen::get_argument(int index, const Local& dst) {
		__ mov(reg_for_arg(index), addr_for_local(dst));
	}
	
	void Codegen::set_return(const Local& src) {
		__ mov(addr_for_local(src), rax);
	}
	
	void Codegen::call(const char* symbol, const Local& retval) {
		__ clear(rax);
		__ call(symbol);
		__ mov(rax, addr_for_local(retval));
	}
}
}