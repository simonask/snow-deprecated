#include "Codegen.h"
#include "ASTNode.h"
#include "lib/Runtime.h"
#include "Internal.h"
#include <stdexcept>
#include <vector>
using namespace std;

#define __ m_Asm->
#define e__ entry_asm->

namespace snow {
namespace x86_64 {
	// stack_frame->`member'
	#define GET_STACK(member) (Address(rbp, (-(int)sizeof(StackFrame))+(int)offsetof(StackFrame, member)))
	
	// temporaries[id]
	#define GET_TEMPORARY(id) (Address(rbp, (-(int)sizeof(StackFrame))-(sizeof(VALUE)*(id+1))))
	
	// reg[index] (reg must contain a pointer to an array of values)
	#define GET_ARRAY_PTR(reg, index) (Address((reg), index * sizeof(VALUE)))
		
	static const Register* arg_regs[] = { &rdi, &rsi, &rdx, &rcx, &r8, &r9 };
	static const uint64_t num_arg_regs = 6;
	
	Codegen::Codegen(ast::FunctionDefinition& def) :
		snow::Codegen(def),
		m_NumLocals(0),
		m_NumTemporaries(0),
		m_NumStackArguments(0) {
		m_Asm = new x86_64::Assembler;
		m_Return = new Label;
		m_Function = new Function;
	}
	
	uint64_t Codegen::reserve_local(const std::string& name) {
		if (has_local(name)) {
			warn("Local `%s' was already reserved in this scope!", name.c_str());
			return local(name);
		}
		uint64_t l = m_NumLocals++;
		m_Function->local_map()[name] = l;
		return l;
	}
	
	bool Codegen::has_local(const std::string& name) {
		return m_Function->has_local(name);
	}
	
	uint64_t Codegen::local(const std::string& name) {
		ASSERT(has_local(name));
		return m_Function->local_map()[name];
	}
	
	void Codegen::get_local(uint64_t id, const Register& reg) {
		__ mov(GET_STACK(locals), reg);
		__ mov(GET_ARRAY_PTR(reg, id), reg);
	}
	
	void Codegen::set_local(const Register& reg, uint64_t id, const Register& tmp) {
		assert(reg != tmp && "Cannot use source register as temporary storage!");
		__ mov(GET_STACK(locals), tmp);
		__ mov(reg, GET_ARRAY_PTR(tmp, id));
	}
	
	void Codegen::get_temporary(uint64_t id, const Register& reg) {
		__ mov(GET_TEMPORARY(id), reg);
	}
	
	void Codegen::set_temporary(const Register& reg, uint64_t id) {
		__ mov(reg, GET_TEMPORARY(id));
	}
	
	RefPtr<CompiledCode> Codegen::compile() {
		RefPtr<x86_64::Assembler> entry_asm = new x86_64::Assembler;
		__ subasm(entry_asm);
		
		if (m_Def.arguments.size() > 0) {
			__ comment("copy arguments to locals");
			__ mov(GET_STACK(arguments), r8);
			__ mov(GET_STACK(locals), r9);
			size_t i = 0;
			for each (iter, m_Def.arguments) {
				auto local = reserve_local((*iter)->name);
				__ mov(GET_ARRAY_PTR(r8, i), rax);
				__ mov(rax, GET_ARRAY_PTR(r9, local));
				++i;
			}
		}
		
		__ comment("function body");
		compile(*m_Def.sequence);
		
		uint64_t return_temporary = reserve_temporary();
		__ comment("function exit");
		__ bind(m_Return);
		set_temporary(rax, return_temporary);
		__ call("snow_leave_scope");
		get_temporary(return_temporary, rax);
		__ leave();
		__ ret();
		
		// Compile the function entry, now that we know all the locals and
		// temporaries.
		{
			e__ comment("function entry");
			int stack_size = sizeof(StackFrame) + sizeof(VALUE)*m_NumTemporaries;
			// maintain 16-byte stack alignment
			stack_size += stack_size % 16;
			e__ enter(stack_size);
			
			e__ mov(rbp, rsi);
			e__ sub(sizeof(StackFrame), rsi);
			e__ call("snow_enter_scope");
		}
		
		RefPtr<CompiledCode> code = __ compile();
		for each (iter, m_Related) {
			code->add_related(*iter);
		}
		return code;
	}
	
	void Codegen::compile(ast::Literal& literal) {
		__ comment(std::string("literal: `")  + literal.string + std::string("'"));
		using ast::Literal;
		
		const char* str = literal.string.c_str();
		
		VALUE val = nil();
		
		switch (literal.type) {
			case Literal::INTEGER_DEC_TYPE:
				val = value(strtoll(str, NULL, 10));
				break;
			case Literal::INTEGER_HEX_TYPE:
				val = value(strtoll(str, NULL, 16));
				break;
			case Literal::INTEGER_BIN_TYPE:
				val = value(strtoll(str, NULL, 2));
				break;
			case Literal::FLOAT_TYPE:
				val = create_float(strtod(str, NULL));
				break;
			case Literal::STRING_TYPE:
				val = create_string(str);
				break;
		}

		__ mov(val, rax);
	}
	
	void Codegen::compile(ast::Identifier& id) {
		__ comment(std::string("identifier: `") + id.name + std::string("'"));
		if (has_local(id.name)) {
			// It's a local from current scope...
			get_local(local(id.name), rax);
		} else {
			// THE PAIN! It's from a parent scope...
			__ mov(rbp, rdi);
			__ sub(sizeof(StackFrame), rdi);
			// TODO/XXX: Symbol storage -- id.name is freed at some point.
			__ mov(id.name.c_str(), rsi);
			__ mov(id.quiet, rdx);
			__ call("snow_get_local");
		}
	}

	void Codegen::compile(ast::Sequence& seq) {
		for each (iter, seq.nodes) {
			(*iter)->compile(*this);
		}
	}

	void Codegen::compile(ast::FunctionDefinition& def) {
		__ comment("function definition");
		RefPtr<Codegen> codegen = new Codegen(def);
		RefPtr<CompiledCode> code = codegen->compile();
		m_Related.push_back(code);
		VALUE func = snow::create_function(code->function());
		__ mov(func, rdi);
		__ mov(rbp, rsi);
		__ sub(sizeof(StackFrame), rsi);
		__ call("snow_set_parent_scope");
		__ mov(func, rax);
	}
	
	void Codegen::compile(ast::Return& ret) {
		__ comment("return");
		if (ret.expression)
			ret.expression->compile(*this);
		else
			__ clear(rax);
		__ jmp(m_Return);
	}

	void Codegen::compile(ast::Assignment& assign) {
		uint64_t l;
		if (has_local(assign.identifier->name))
			l = local(assign.identifier->name);
		else
			l = reserve_local(assign.identifier->name);
		
		assign.expression->compile(*this);
		__ comment(std::string("assignment: ") + assign.identifier->name);
		set_local(rax, l);
	}

	void Codegen::compile(ast::IfCondition& cond) {
		RefPtr<Label> test_cond = new Label;
		RefPtr<Label> after = new Label;
		
		__ bind(test_cond);
		__ comment("if cond");
		cond.expression->compile(*this);
		__ mov(rax, rdi);
		__ call("snow_eval_truth");
		__ cmp(0, rax);
		__ j(CC_EQUAL, after);
		__ comment("if body");
		cond.if_true->compile(*this);
		__ bind(after);
	}

	void Codegen::compile(ast::IfElseCondition& cond) {
		RefPtr<Label> test_cond = new Label;
		RefPtr<Label> if_true = new Label;
		RefPtr<Label> if_false = new Label;
		RefPtr<Label> after = new Label;
		
		__ bind(test_cond);
		__ comment("if-else cond");
		cond.expression->compile(*this);
		__ mov(rax, rdi);
		__ call("snow_eval_truth");
		__ cmp(0, rax);
		__ j(CC_EQUAL, if_false);
		__ bind(if_true);
		__ comment("if true");
		cond.if_true->compile(*this);
		__ jmp(after);
		__ bind(if_false);
		__ comment("if false");
		cond.if_false->compile(*this);
		__ bind(after);
	}

	void Codegen::compile(ast::Call& call) {
		std::vector<uint64_t> temporaries(call.arguments ? call.arguments->nodes.size() : 0);
		uint64_t num_args = 0;
		
		if (call.arguments) {
			num_args = call.arguments->nodes.size();
			for each (iter, call.arguments->nodes) {
				uint64_t tmp = reserve_temporary();
				__ comment("argument for call");
				(*iter)->compile(*this);
				__ mov(rax, GET_TEMPORARY(tmp));
				temporaries.push_back(tmp);
			}
		}
		
		__ comment("call target");
		call.object->compile(*this);
		__ mov(rax, *arg_regs[0]);
		__ mov(Immediate(num_args), *arg_regs[1]);
		
		__ comment("fetch arguments for call");
		auto iter = temporaries.begin();
		for (uint64_t i = 0; i < num_args; ++i) {
			if (iter == temporaries.end())
				break;
			__ mov(GET_TEMPORARY(*iter), *arg_regs[i+2]);
			// TODO: Stack arguments
			iter++;
		}
		
		__ clear(rax);
		__ comment("call");
		__ call("snow_call");
	}
	
	void Codegen::compile(ast::MethodCall& call) {
		std::vector<uint64_t> temporaries(call.arguments ? call.arguments->nodes.size() : 0);
		uint64_t num_args = 0;
		
		if (call.arguments) {
			num_args = call.arguments->nodes.size();
			for each (iter, call.arguments->nodes) {
				uint64_t tmp = reserve_temporary();
				__ comment("argument for method-call");
				(*iter)->compile(*this);
				__ mov(rax, GET_TEMPORARY(tmp));
				temporaries.push_back(tmp);
			}
		}
		
		__ comment("self for method-call");
		call.self->compile(*this);
		__ mov(rax, *arg_regs[0]);
		__ comment(std::string("method name: ") + call.message->name);
		__ mov(Immediate(call.message->name.c_str()), *arg_regs[1]);
		__ comment("num args for method-call");
		__ mov(Immediate(num_args), *arg_regs[2]);
		
		__ comment("fetch arguments for method-call");
		auto iter = temporaries.begin();
		for (uint64_t i = 0; i < num_args; ++i) {
			if (iter == temporaries.end())
				break;
			__ mov(GET_TEMPORARY(*iter), *arg_regs[i+3]);
			// TODO: Stack arguments
			iter++;
		}
		
		__ clear(rax);
		__ comment("method-call");
		__ call("snow_call_method");
	}

	void Codegen::compile(ast::Send& send) {
		uint64_t message_tmp = reserve_temporary();

		send.message->compile(*this);
		__ mov(rax, rdi);
		__ call("snow_value_to_string");
		__ mov(rax, GET_TEMPORARY(message_tmp));
		
		send.self->compile(*this);
		__ mov(rax, rdi);
		__ mov(GET_TEMPORARY(message_tmp), rsi);
		__ call("snow_send");
	}

	void Codegen::compile(ast::Loop& loop) {
		RefPtr<Label> test_cond = new Label;
		RefPtr<Label> body = new Label;
		RefPtr<Label> after = new Label;
		
		__ bind(test_cond);
		__ comment("loop cond");
		loop.expression->compile(*this);
		__ mov(rax, rdi);
		__ call("snow_eval_truth");
		__ cmp(0, rax);
		__ j(CC_EQUAL, after);
		__ bind(body);
		__ comment("loop body");
		loop.while_true->compile(*this);
		__ jmp(test_cond);
		__ bind(after);
	}

}
}