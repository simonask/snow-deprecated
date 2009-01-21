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
	static const uint64_t num_arg_regs = 6;
	
//	static const Register* tmp_regs[] = { &rax,  &r10,  &r11,  &rbx, &r12, &r13, &r14, &r15 };
	static const bool preserve_regs[] = { false, false, false, true, true, true, true, true };
	
	template <typename T>
	void set_argument(x86_64::Codegen& codegen, uint64_t idx, T value) {
		if (idx < num_arg_regs) {
			codegen.m_Asm->mov(value, *arg_regs[idx]);
		} else {
			uint64_t stack_idx = idx - num_arg_regs;
			if (stack_idx+1 > codegen.m_NumStackArguments) {
				codegen.m_NumStackArguments = stack_idx+1;
			}
			codegen.m_Asm->mov(rsp, r11);
			codegen.m_Asm->mov(value, rax);
			codegen.m_Asm->mov(rax, Address(r11, stack_idx*sizeof(VALUE)));
		}
	}
	
	template <typename T>
	void get_argument(x86_64::Codegen& codegen, uint64_t idx, T target) {
		if (idx < num_arg_regs) {
			codegen.m_Asm->mov(*arg_regs[idx], target);
		} else {
			uint64_t stack_idx = idx - num_arg_regs + 2;
			codegen.m_Asm->mov(Address(rbp, stack_idx*sizeof(VALUE)), target);
		}
	}
	
	Codegen::Codegen(ast::FunctionDefinition& def) : snow::Codegen(def), m_NextTemporary(0), m_NumStackArguments(0) {
		m_Asm = new x86_64::Assembler;
		m_Scope = new Scope;
	}
	
	int Codegen::offset_for_locals() {
		return offset_for_stack_frame() - num_locals() * sizeof(VALUE);
	}
	
	int Codegen::offset_for_local(const Scope::Local& local) {
		return offset_for_locals() + local.index * sizeof(VALUE);
	}
	
	Address Codegen::address_for_local(const Scope::Local& local) {
		return Address(rbp, offset_for_local(local));
	}
	
	int Codegen::offset_for_temporaries() {
		return offset_for_locals();
	}
	
	int Codegen::offset_for_temporary(int id) {
		// NB: Temporaries are in reverse order
		return offset_for_temporaries() - (id+1) * sizeof(VALUE);
	}
	
	Address Codegen::address_for_temporary(int id) {
		return Address(rbp, offset_for_temporary(id));
	}
	
	Address Codegen::create_temporary() {
		return address_for_temporary(m_NextTemporary++);
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
		__ mov((uint64_t)m_Def.arguments.size(), rax);
		__ mov(rax, Address(rbp, offset_for_stack_frame() + offsetof(StackFrame, num_named_args)));
		__ mov(num_locals, rcx);    // frame->num_locals = num_locals
		__ mov(rcx, Address(rbp, offset_for_stack_frame() + offsetof(StackFrame, num_locals)));
		__ mov(rbp, rax);
		__ add(offset_for_stack_frame(), rax);
		__ mov(rax, rdi);           // StackFrame* is first argument below
		__ mov(rbp, rax);
		__ add(offset_for_locals(), rax);// frame->locals = %rbp - stack_frame - locals
		__ mov(rax, Address(rbp, offset_for_stack_frame()+offsetof(StackFrame, locals)));
		__ call("snow_init_stack_frame");     // initialize with runtime info

		// Copy arguments into locals
		RefPtr<Label> copy_cond = new Label;
		RefPtr<Label> copy_done = new Label;
		// init
		__ clear(rax);
		__ mov((uint64_t)m_Def.arguments.size(), r8);
		__ mov(Address(rbp, offset_for_stack_frame() + offsetof(StackFrame, num_args)), r9);
		__ mov(Address(rbp, offset_for_stack_frame() + offsetof(StackFrame, args)), r10);
		__ mov(Address(rbp, offset_for_stack_frame() + offsetof(StackFrame, locals)), r11);
		// conditions (rax < num_named_args && rax < num_args)
		__ bind(copy_cond);
		__ cmp(r8, rax);
		__ j(CC_ABOVE, copy_done);
		__ cmp(r9, rax);
		__ j(CC_ABOVE, copy_done);
		// do copy
		__ mov(SIB(r10, rax, SIB::SCALE_8), rcx);
		__ mov(rcx, SIB(r11, rax, SIB::SCALE_8));
		__ jmp(copy_cond);
		// done
		__ bind(copy_done);
		__ clear(rax);
	}
	
	void Codegen::find_locals() {
		// Arguments are locals
		// (some arguments are in registers, so we might as well preserve them
		// all on the stack)
		for each (iter, m_Def.arguments) {
			m_Scope->add_local((*iter)->name);
		}
		
		// Look for other locals
		m_Def.sequence->export_locals(*m_Scope);
	}
	
	int Codegen::num_locals() const {
		return m_Scope->num_locals();
	}
	
	RefPtr<CompiledCode> Codegen::compile() {
		find_locals();
		
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
		
		RefPtr<x86_64::Assembler> enter_asm = new x86_64::Assembler;
		__ subasm(enter_asm);

		establish_stack_frame(m_Asm, num_locals);
		
		compile(*m_Def.sequence);
		
		int temporaries_size = (m_NextTemporary+m_NumStackArguments) * sizeof(VALUE);
		temporaries_size += temporaries_size % 16;
		if (temporaries_size)
			enter_asm->sub(temporaries_size, rsp);
		
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
		
		const char* str = literal.string.c_str();
		
		switch (literal.type) {
			case Literal::INTEGER_DEC_TYPE:
				__ mov(value(strtoll(str, NULL, 10)), rax);
				break;
			case Literal::INTEGER_HEX_TYPE:
				__ mov(value(strtoll(str, NULL, 16)), rax);
				break;
			case Literal::INTEGER_BIN_TYPE:
				__ mov(value(strtoll(str, NULL, 2)), rax);
				break;
			case Literal::FLOAT_TYPE:
				__ mov(create_float(strtod(str, NULL)), rax);
				break;
			case Literal::STRING_TYPE:
				__ mov(create_string(str), rax);
				break;
		}
	}
	
	void Codegen::compile(ast::Identifier& id) {
		// XXX: Check parent scopes, do something for undefined, etc.
		if (id.name == "self") {
			__ mov(Address(rbp, offset_for_stack_frame() + offsetof(StackFrame, self)), rax);
		} else {
			__ mov(address_for_local(m_Scope->get_local(id.name)), rax);
		}
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

	void Codegen::compile(ast::IfCondition& cond) {
		RefPtr<Label> test_cond = new Label;
		RefPtr<Label> after = new Label;
		
		__ bind(test_cond);
		cond.expression->compile(*this);
		__ mov(rax, rdi);
		__ call("snow_eval_truth");
		__ cmp(0, rax);
		__ j(CC_EQUAL, after);
		cond.if_true->compile(*this);
		__ bind(after);
	}

	void Codegen::compile(ast::IfElseCondition& cond) {
		RefPtr<Label> test_cond = new Label;
		RefPtr<Label> if_true = new Label;
		RefPtr<Label> if_false = new Label;
		RefPtr<Label> after = new Label;
		
		__ bind(test_cond);
		cond.expression->compile(*this);
		__ mov(rax, rdi);
		__ call("snow_eval_truth");
		__ cmp(0, rax);
		__ j(CC_EQUAL, if_false);
		__ bind(if_true);
		cond.if_true->compile(*this);
		__ jmp(after);
		__ bind(if_false);
		cond.if_false->compile(*this);
		__ bind(after);
	}

	void Codegen::compile(ast::Call& call) {
		std::list<Address> temporaries;
		uint64_t num_args;
		
		if (call.arguments) {
			num_args = call.arguments->nodes.size();
			uint64_t i = 0;
			for each (iter, call.arguments->nodes) {
				Address tmp = create_temporary();
				(*iter)->compile(*this);
				__ mov(rax, tmp);
				temporaries.push_back(tmp);
				++i;
			}
		}
		
		call.object->compile(*this);
		set_argument(*this, 0, rax);
		set_argument(*this, 1, Immediate(num_args));
		
		auto iter = temporaries.begin();
		for (uint64_t i = 2; i < num_args+2; ++i) {
			if (iter == temporaries.end())
				break;
			set_argument(*this, i, *iter);
			iter++;
		}
		
		__ clear(rax);
		__ call("snow_call");
	}
	
	void Codegen::compile(ast::MethodCall& call) {
		std::list<Address> temporaries;
		uint64_t num_args;
		
		num_args = call.arguments->nodes.size();
		uint64_t i = 0;
		for each (iter, call.arguments->nodes) {
			Address tmp = create_temporary();
			(*iter)->compile(*this);
			__ mov(rax, tmp);
			temporaries.push_back(tmp);
			++i;
		}
		
		call.self->compile(*this);
		set_argument(*this, 0, rax);
		set_argument(*this, 1, Immediate(call.message->name.c_str()));
		set_argument(*this, 2, Immediate(num_args));
		
		auto iter = temporaries.begin();
		for (uint64_t i = 3; i < num_args+3; ++i) {
			if (iter == temporaries.end())
				break;
			set_argument(*this, i, *iter);
			iter++;
		}
		
		__ clear(rax);
		__ call("snow_call_method");
	}

	void Codegen::compile(ast::Send& send) {
		Address message = create_temporary();

		send.message->compile(*this);

		__ mov(rax, rdi);
		__ call("snow_value_to_string");
		__ mov(rax, message);
		
		send.self->compile(*this);
		__ mov(rax, rdi);
		__ mov(message, rsi);
		__ call("snow_send");
	}

	void Codegen::compile(ast::Loop& loop) {
		RefPtr<Label> test_cond = new Label;
		RefPtr<Label> body = new Label;
		RefPtr<Label> after = new Label;
		
		__ bind(test_cond);
		loop.expression->compile(*this);
		__ mov(rax, rdi);
		__ call("snow_eval_truth");
		__ cmp(0, rax);
		__ j(CC_EQUAL, after);
		__ bind(body);
		loop.while_true->compile(*this);
		__ jmp(test_cond);
		__ bind(after);
	}

}
}