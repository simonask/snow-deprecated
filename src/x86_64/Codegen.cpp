#include "Codegen.h"
#include <stdexcept>

#define __ m_Assembler.

namespace snot {
namespace x86_64 {
	static const Register* arg_regs[] = { &rdi, &rsi, &rdx, &rcx, &r8, &r9 };
	
	static const Register* tmp_regs[] = { &rax,  &r10,  &r11,  &rbx, &r12, &r13, &r14, &r15 };
	static const bool preserve_regs[] = { false, false, false, true, true, true, true, true };
	
	static inline Address addr_for_local(const Scope::Local& local) {
		return Address(rbp, (local.index + 1) * -8, true);
	}
	
	static inline const Register& reg_for_arg(int index) {
		return *arg_regs[index];
	}
	
	static inline const Register& reg_for_tmp(int index) {
		return *tmp_regs[index];
	}
	
	void Codegen::preserve_tmp_reg(int index) {
		if (preserve_regs[index]) {
			m_CurrentScope.enter_asm->push(*tmp_regs[index]);
			
			m_PreservedTempRegisters.push_back(tmp_regs[index]);
		}
	}
	
	CompiledCode Codegen::compile() {
		return __ compile();
	}
	
	Scope Codegen::function_entry(int num_locals) {
		// maintain 16-byte stack alignment
		int stack_size = 8 * (num_locals + (num_locals % 2));
		
		// enter uses a 16-bit immediate for stack size
		if (stack_size < 1 << 16)
			__ enter(stack_size);
		else {
			// ... which may be too little
			__ push(rbp);
			__ mov(rsp, rbp);
			__ sub(stack_size, rsp);
		}
		
		x86_64::Assembler* enter_asm = new x86_64::Assembler;
		x86_64::Assembler* leave_asm = new x86_64::Assembler;

		if (m_CurrentScope.enter_asm)
			m_ScopeData.push_back(m_CurrentScope);
		m_CurrentScope = ScopeData(enter_asm, leave_asm);
		
		__ subasm(enter_asm);
		
		return Scope();
	}
	
	void Codegen::function_return() {
		for (auto iter = riterate(m_PreservedTempRegisters); iter; ++iter) {
			m_CurrentScope.leave_asm->pop(**iter);
		}
		
		__ subasm(m_CurrentScope.leave_asm);
		__ leave();
		__ ret();
		
		m_CurrentScope = ScopeData(m_ScopeData.back());
		m_ScopeData.pop_back();
	}
	
	void Codegen::set_argument(int index, const Scope::Local& src) {
		__ mov(addr_for_local(src), reg_for_arg(index));
	}
	
	void Codegen::set_argument(int index, const Scope::Temporary& src) {
		__ mov(reg_for_tmp(src.index), reg_for_arg(index));
	}
	
	void Codegen::set_argument(int index, const void* ptr) {
		__ mov((const char*)ptr, reg_for_arg(index));
	}
	
	void Codegen::set_argument(int index, int immediate) {
		__ mov(immediate, reg_for_arg(index));
	}
	
	void Codegen::get_argument(int index, const Scope::Local& dst) {
		__ mov(reg_for_arg(index), addr_for_local(dst));
	}
	
	void Codegen::get_argument(int index, const Scope::Temporary& dst) {
		preserve_tmp_reg(dst.index);
		__ mov(reg_for_arg(index), reg_for_tmp(dst.index));
	}
	
	void Codegen::set_return(const Scope::Local& src) {
		__ mov(addr_for_local(src), rax);
	}
	
	void Codegen::set_return(const Scope::Temporary& src) {
		if (reg_for_tmp(src.index) != rax)
			__ mov(reg_for_tmp(src.index), rax);
	}
	
	void Codegen::call(const char* symbol, const Scope::Local& retval) {
		__ clear(rax);
		__ call(symbol);
		__ mov(rax, addr_for_local(retval));
	}
	
	void Codegen::call(const char* symbol, const Scope::Temporary& retval) {
		__ clear(rax);
		__ call(symbol);
		if (reg_for_tmp(retval.index) != rax)
			__ mov(rax, reg_for_tmp(retval.index));
	}
}
}