#include "Codegen.h"
#include <stdexcept>
#include <vector>
using namespace std;

#define __ m->

namespace snow {
namespace x86_64 {
	static const Register* arg_regs[] = { &rdi, &rsi, &rdx, &rcx, &r8, &r9 };
	
	static const Register* tmp_regs[] = { &rax,  &r10,  &r11,  &rbx, &r12, &r13, &r14, &r15 };
	static const bool preserve_regs[] = { false, false, false, true, true, true, true, true };
	
	static inline Address addr_for_local(const Scope::Local& local) {
		return Address(rbp, -sizeof(StackFrame) - (local.index * sizeof(VALUE)), true);
	}
	
	static inline const Register& reg_for_arg(int index) {
		return *arg_regs[index];
	}
	
	static inline const Register& reg_for_tmp(int index) {
		return *tmp_regs[index];
	}
	
	static inline int offset_for_stack_frame() {
		return -(int)sizeof(StackFrame);
	}
	
	static inline int offset_for_locals(int num_locals) {
		return offset_for_stack_frame() - (int)(num_locals * sizeof(VALUE));
	}
	
	static inline int offset_for_local(int index, int num_locals) {
		return offset_for_locals(num_locals) + index*sizeof(VALUE);
	}
	
	void Codegen::preserve_tmp_reg(int index) {
		if (preserve_regs[index]) {
			m_PreservedTempRegisters.push_back(tmp_regs[index]);
		}
	}
	
	void Codegen::establish_stack_frame(const RefPtr<x86_64::Assembler>& m, int num_locals) {
		/*
			STACK LAYOUT:
			
			struct StackFrame
				StackFrame* previous
				uint64_t num_locals
				VALUE* locals
			VALUE* local1 = self
			VALUE* local2 = arg1
			VALUE* local3 = arg2
			VALUE* local4 = assignment1
			VLAUE* local5 = assignment2
			...
			VALUE* local(num_locals-1)
		*/
		
		// Copy arguments to stack.
		Label* copy_args_cond = new Label;
		Label* copy_args_done = new Label;
		__ mov(rdi, Address(rbp, offset_for_local(0, num_locals)));	// "self"
		__ mov(rsi, rcx);  // rcx = num_args
		__ mov(sizeof(VALUE), rax);
		__ imul(rax, rcx); // rcx = num_args * sizeof(VALUE)
		__ clear(rax);     // rax = 0
		__ clear(r11);
		__ cmp(rdx, rax);
		__ j(CC_EQUAL, *copy_args_done);  // if args == NULL, abort mission
		__ bind(*copy_args_cond);
		__ cmp(rcx, rax);
		__ j(CC_EQUAL, *copy_args_done);
		__ mov(rdx, r8);   // r8 = &args[0]
		__ add(rax, r8);   // r8 = &args[rax]
		__ mov(Address(r8), r10); // r10 = *r8
		__ mov(rbp, r9);
		__ add(offset_for_locals(num_locals), r9);
		__ add(rax, r9);   // r9 = &locals[rax]
		__ mov(r10, Address(r9));  // *r9 = r10
		__ add(Immediate(sizeof(VALUE)), rax); // rax = &args[i]
		__ inc(r11);
		__ jmp(*copy_args_cond);
		__ bind(*copy_args_done);
		
		// Create stack frame info
		__ mov(num_locals, rcx);    // frame->num_locals = num_locals
		__ mov(rcx, Address(rbp, offset_for_stack_frame()+offsetof(StackFrame, num_locals)));
		__ mov(rbp, rax);
		__ add(offset_for_stack_frame(), rax);
		__ mov(rax, rdi);           // StackFrame* is first argument below
		__ mov(rbp, rax);
		__ add(offset_for_locals(num_locals), rax);// frame->locals = %rbp - stack_frame - locals
		__ mov(rax, Address(rbp, offset_for_stack_frame()+offsetof(StackFrame, locals)));
		__ call("snow_create_stack_frame");     // initialize with runtime info

	}
	
	void Codegen::find_locals(const ast::FunctionDefinition& def, Scope::LocalList& locals) {
		// First local is always self.
		locals.add("self");
		
		// Arguments are locals
		// (some arguments are in registers, so we might as well preserve them
		// all on the stack)
		for each (iter, def.arguments) {
			locals.add((*iter)->name);
		}
		
		// Look for other locals
		def.sequence->export_locals(locals);
	}
	
	RefPtr<CompiledCode> Codegen::compile(const ast::FunctionDefinition& def) {
		RefPtr<x86_64::Assembler> m = new x86_64::Assembler;
		RefPtr<Scope> scope = new Scope;
		
		// Related CompiledCodes are functions defined within the current
		// function definition.
		vector<RefPtr<CompiledCode>> related;
		
		find_locals(def, scope->locals());
		
		int num_locals = scope->locals().size();
		debug("num_locals: %d", num_locals);
		
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

		establish_stack_frame(m, num_locals);
		
		#ifdef DEBUG
		// Clear locals
		__ clear(rax);
		for (int i = 0; i < num_locals; ++i) {
			__ mov(rax, Address(rbp, offset_for_local(i, num_locals)));
		}
		#endif
		
		RefPtr<x86_64::Assembler> enter_asm = new x86_64::Assembler;
		
		// Preserve registers
		__ subasm(enter_asm);
		
		for each (iter, def.sequence->nodes) {
		/*
			HERE
		*/
		}
		
		RefPtr<x86_64::Assembler> leave_asm = new x86_64::Assembler;
		
		// Restore non-volatile registers
		for (auto iter = riterate(m_PreservedTempRegisters); iter; ++iter) {
			leave_asm->pop(**iter);
		}
		m_PreservedTempRegisters.clear();
		
		__ subasm(leave_asm);
		__ leave();
		__ ret();
		
		return __ compile();
	}
}
}