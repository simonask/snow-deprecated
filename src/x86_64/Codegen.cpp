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
	
	static inline Address address_for_local(const Scope::Local& local) {
		return Address(rbp, offset_for_local(local.index, local.scope->locals().size()));
	}
	
	void Codegen::preserve_tmp_reg(int index) {
		if (preserve_regs[index]) {
			m_PreservedTempRegisters.push_back(tmp_regs[index]);
		}
	}
	
	void Codegen::establish_stack_frame(const RefPtr<x86_64::Assembler>& m, int num_locals) {
		/*
			STACK LAYOUT:
			
			struct StackFrame {
				StackFrame* previous;
				VALUE self;
				VALUE call_self;
				uint64_t num_args;
				VALUE* args;
				uint64_t num_locals;
				VALUE* locals;
			};
			VALUE local(num_locals-1)
			...
			VALUE local3
			VALUE local2
			VALUE local1
		*/
		
		// Init stack frame info
		__ mov(rdi, Address(rbp, offset_for_stack_frame() + offsetof(StackFrame, call_self)));
		__ mov(rsi, Address(rbp, offset_for_stack_frame() + offsetof(StackFrame, num_args)));
		__ mov(rdx, Address(rbp, offset_for_stack_frame() + offsetof(StackFrame, args)));
		__ mov(num_locals, rcx);    // frame->num_locals = num_locals
		__ mov(rcx, Address(rbp, offset_for_stack_frame() + offsetof(StackFrame, num_locals)));
		__ mov(rbp, rax);
		__ add(offset_for_stack_frame(), rax);
		__ mov(rax, rdi);           // StackFrame* is first argument below
		__ mov(rbp, rax);
		__ add(offset_for_locals(num_locals), rax);// frame->locals = %rbp - stack_frame - locals
		__ mov(rax, Address(rbp, offset_for_stack_frame()+offsetof(StackFrame, locals)));
		__ call("snow_init_stack_frame");     // initialize with runtime info

	}
	
	void Codegen::find_locals(const ast::FunctionDefinition& def, Scope& scope) {
		// First local is always self.
		scope.add_local("self");
		
		// Arguments are locals
		// (some arguments are in registers, so we might as well preserve them
		// all on the stack)
		for each (iter, def.arguments) {
			scope.add_local((*iter)->name);
		}
		
		// Look for other locals
		def.sequence->export_locals(scope);
	}
	
	RefPtr<CompiledCode> Codegen::compile(const ast::FunctionDefinition& def) {
		RefPtr<x86_64::Assembler> m = new x86_64::Assembler;
		RefPtr<Scope> scope = new Scope;
		
		// Related CompiledCodes are functions defined within the current
		// function definition.
		vector<RefPtr<CompiledCode>> related;
		
		find_locals(def, *scope);
		
		int num_locals = scope->num_locals();
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
			ast::Node& node(**iter);
			if (node.is_a<ast::Assignment>()) {
				ast::Assignment* a = node.as<ast::Assignment>();
				Scope::Local l = scope->get_local(a->identifier->name);
				__ mov(value(123LL), rax);
				__ mov(rax, address_for_local(l));
			}
		}
		
		RefPtr<x86_64::Assembler> leave_asm = new x86_64::Assembler;
		
		for each (iter, scope->locals()) {
			__ mov(address_for_local(iter->second), rdi);
			__ call("snow_destroy");
		}
		
		// Restore non-volatile registers
		for (auto iter = riterate(m_PreservedTempRegisters); iter; ++iter) {
			leave_asm->pop(**iter);
		}
		m_PreservedTempRegisters.clear();
		
		__ subasm(leave_asm);
		
		__ debug_break();
		__ leave();
		__ ret();
		
		return __ compile();
	}
}
}