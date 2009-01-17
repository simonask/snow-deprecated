#include "Codegen.h"
#include "ASTNode.h"
#include "lib/Runtime.h"
#include <stdexcept>
#include <vector>
using namespace std;

#define __ m_Asm->

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
	
	Codegen::Codegen(ast::FunctionDefinition& def) : snow::Codegen(def) {
		m_Asm = new x86_64::Assembler;
		m_Scope = new Scope;
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
	
	RefPtr<CompiledCode> Codegen::compile() {
		find_locals(m_Def, *m_Scope);
		
		int num_locals = m_Scope->num_locals();
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

		establish_stack_frame(m_Asm, num_locals);
		
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
		
		compile(*m_Def.sequence);
		
		RefPtr<x86_64::Assembler> leave_asm = new x86_64::Assembler;
		
		for each (iter, m_Scope->locals()) {
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
		
		RefPtr<CompiledCode> code = __ compile();
		for each (iter, m_Related) {
			code->add_related(*iter);
		}
		return code;
	}
	
	void Codegen::compile(ast::Literal& literal) {
		using ast::Literal;
		switch (literal.type) {
			case Literal::INTEGER_TYPE:
			case Literal::INTEGER_HEX_TYPE:
			case Literal::INTEGER_BIN_TYPE:
			case Literal::FLOAT_TYPE:
			case Literal::STRING_TYPE:
			__ mov(value(123LL), rax);
			break;
		}
	}
	
	void Codegen::compile(ast::Identifier& id) {
		// XXX: Check parent scopes, do something for undefined, etc.
		__ mov(address_for_local(m_Scope->get_local(id.name)), rax);
	}

	void Codegen::compile(ast::Sequence& seq) {
		for each (iter, seq.nodes) {
			(*iter)->compile(*this);
		}
	}

	void Codegen::compile(ast::FunctionDefinition& def) {
		RefPtr<Codegen> codegen = new Codegen(def);
		RefPtr<CompiledCode> code = codegen->compile();
		m_Related.push_back(code);
		VALUE func = snow::create_function(code->function());
		__ mov(func, rax);
	}

	void Codegen::compile(ast::Assignment& assign) {
		const Scope::Local& local = m_Scope->get_local(assign.identifier->name);
		assign.expression->compile(*this);
		__ mov(rax, address_for_local(local));
	}

	void Codegen::compile(ast::IfCondition&) {
		
	}

	void Codegen::compile(ast::IfElseCondition&) { }

	void Codegen::compile(ast::Call&) { }

	void Codegen::compile(ast::Send&) { }

	void Codegen::compile(ast::Loop&) { }

}
}