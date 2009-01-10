#include "Codegen.h"
#include <stdexcept>

#define __ m_Assembler.

namespace snow {
namespace x86_64 {
	static const Register* arg_regs[] = { &rdi, &rsi, &rdx, &rcx, &r8, &r9 };
	
	static const Register* tmp_regs[] = { &rax,  &r10,  &r11,  &rbx, &r12, &r13, &r14, &r15 };
	static const bool preserve_regs[] = { false, false, false, true, true, true, true, true };
	
	static inline Address addr_for_local(const Codegen::Local& local) {
		return Address(rbp, -sizeof(StackFrame) - (local.index * sizeof(VALUE)), true);
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
	
	RefPtr<CompiledCode> Codegen::compile() {
		return __ compile();
	}
	
	Scope Codegen::function_entry(int num_locals) {
		/*
			STACK LAYOUT:
			
			struct StackFrame
				StackFrame* previous
				uint64_t num_locals
				VALUE* locals
			VALUE* local1 = self
			VALUE* local2 = arg1
			VALUE* local3 = arg2
			VALUE* local4
			...
			VALUE* local(num_locals-1)
		*/
		
		int stack_size = sizeof(StackFrame) + sizeof(VALUE)*num_locals;
		// maintain 16-byte stack alignment
		stack_size += stack_size % 16;
		
		// enter uses a 16-bit immediate for stack size
		if (stack_size < 1 << 16)
			__ enter(stack_size);
		else {
			// ... which may be too little
			__ push(rbp);
			__ mov(rsp, rbp);
			__ sub(stack_size, rsp);
		}
		
		// Establish scope
		x86_64::Assembler* enter_asm = new x86_64::Assembler;
		x86_64::Assembler* leave_asm = new x86_64::Assembler;

		if (m_CurrentScope.enter_asm)
			m_ScopeData.push_back(m_CurrentScope);
		m_CurrentScope = ScopeData(enter_asm, leave_asm);
		
		
		int stack_frame_offset = -(int)sizeof(StackFrame);
		
		// Create stack frame
		__ mov(rdi, rbx);           // preserve first argument
		__ mov(num_locals, rcx);    // frame->num_locals = num_locals
		__ mov(rcx, Address(rbp, stack_frame_offset+offsetof(StackFrame, num_locals)));
		__ mov(rbp, rax);
		__ add(stack_frame_offset, rax);
		__ mov(rax, rdi);           // StackFrame* is first argument below
		__ sub(sizeof(VALUE)*num_locals, rax);  // frame->locals = %rbp - stack_frame - locals
		__ mov(rax, Address(rbp, stack_frame_offset+offsetof(StackFrame, locals)));
		__ call("snow_create_stack_frame");     // initialize with runtime info
		__ mov(rbx, rdi);           // restore first argument
		
		// Preserve registers
		__ subasm(enter_asm);
		
		return Scope();
	}
	
	void Codegen::function_return() {
		// Restore non-volatile registers
		for (auto iter = riterate(m_PreservedTempRegisters); iter; ++iter) {
			m_CurrentScope.leave_asm->pop(**iter);
		}
		m_PreservedTempRegisters.clear();
		
		__ subasm(m_CurrentScope.leave_asm);
		__ leave();
		__ ret();
		
		// Restore parent scope, if any
		if (m_ScopeData.size() > 0) {
			m_CurrentScope = ScopeData(m_ScopeData.back());
			m_ScopeData.pop_back();
		} else {
			m_CurrentScope = ScopeData();
		}
	}
	
	void Codegen::set_local(const Codegen::Local& dst, const Codegen::Local& src) {
		if (dst != src) {
			__ mov(addr_for_local(src), rbx);
			__ mov(rbx, addr_for_local(dst));
		}
	}
	
	void Codegen::set_local(const Codegen::Local& dst, VALUE constant) {
		__ mov(Immediate((int64_t)constant), addr_for_local(dst));
	}
	
	void Codegen::set_argument(int index, const Codegen::Local& src) {
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
	
	void Codegen::get_argument(int index, const Codegen::Local& dst) {
		__ mov(reg_for_arg(index), addr_for_local(dst));
	}
	
	void Codegen::get_argument(int index, const Scope::Temporary& dst) {
		preserve_tmp_reg(dst.index);
		__ mov(reg_for_arg(index), reg_for_tmp(dst.index));
	}
	
	void Codegen::set_return(const Codegen::Local& src) {
		__ mov(addr_for_local(src), rax);
	}
	
	void Codegen::set_return(const Scope::Temporary& src) {
		if (reg_for_tmp(src.index) != rax)
			__ mov(reg_for_tmp(src.index), rax);
	}
	
	void Codegen::call(const char* symbol) {
		__ clear(rax);
		__ call(symbol);
	}
	
	void Codegen::call(const char* symbol, const Codegen::Local& retval) {
		call(symbol);
		__ mov(rax, addr_for_local(retval));
	}
	
	void Codegen::call(const char* symbol, const Scope::Temporary& retval) {
		call(symbol);
		if (reg_for_tmp(retval.index) != rax)
			__ mov(rax, reg_for_tmp(retval.index));
	}
}
}