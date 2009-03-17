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
		m_NumStackArguments(0),
		m_NumTemporaries(0) {
		m_LocalMap = new LocalMap;
		m_Asm = new x86_64::Assembler;
		m_Return = new Label;
	}
	
	uint64_t Codegen::reserve_temporary() {
		if (m_FreeTemporaries.size() > 0) {
			uint64_t t = m_FreeTemporaries.back();
			m_FreeTemporaries.pop_back();
			return t;
		}
		return m_NumTemporaries++;
	}
	
	void Codegen::free_temporary(uint64_t id) {
		m_FreeTemporaries.push_back(id);
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
	
	RefPtr<CompiledCode> Codegen::compile() {
		RefPtr<x86_64::Assembler> entry_asm = new x86_64::Assembler;
		__ subasm(entry_asm);
		
		if (m_Def.arguments.size() > 0) {
			__ comment("copy arguments to locals");
			__ mov(GET_STACK(arguments), r8);
			__ mov(GET_STACK(locals), r9);
			size_t i = 0;
			for each (iter, m_Def.arguments) {
				auto local = m_LocalMap->define_local((*iter)->name);
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
		__ mov(rax, GET_TEMPORARY(return_temporary));
		__ call("snow_leave_scope");
		__ mov(GET_TEMPORARY(return_temporary), rax);
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
		code->set_local_map(m_LocalMap);
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
				// TODO: Doubles
				val = value(strtof(str, NULL));
				break;
			case Literal::STRING_TYPE:
				val = create_string(str);
				break;
			case Literal::TRUE_TYPE:
				val = value(true);
				break;
			case Literal::FALSE_TYPE:
				val = value(false);
				break;
			case Literal::NIL_TYPE:
				val = nil();
                break;
		}

		__ mov(val, rax);
	}
	
	void Codegen::compile(ast::Identifier& id) {
		__ comment(std::string("identifier: `") + id.name + std::string("'"));
		if (m_LocalMap->has_local(id.name)) {
			// It's a local from current scope...
			get_local(m_LocalMap->local(id.name), rax);
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
		VALUE func = new Function(*code);
		__ mov(func, rdi);
		__ mov(GET_STACK(scope), rsi);
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
		if (m_LocalMap->has_local(assign.identifier->name))
			l = m_LocalMap->local(assign.identifier->name);
		else
			l = m_LocalMap->define_local(assign.identifier->name);
		
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
		auto self_tmp = reserve_temporary();
		
		__ comment("self for call");
		call.self->compile(*this);
		__ mov(rax, GET_TEMPORARY(self_tmp));
		
		// evaluate arguments and store temporaries
		auto num_args = call.arguments->length();
		uint64_t args_tmp[num_args];
		size_t i = 0;
		for each (arg_iter, call.arguments->nodes) {
			args_tmp[i] = reserve_temporary();
			__ comment(string_printf("argument %d", i));
			(*arg_iter)->compile(*this);
			__ mov(rax, GET_TEMPORARY(args_tmp[i]));
			++i;
		}
		
		auto function_tmp = self_tmp;
		if (call.member) {
			__ comment("method call");
			function_tmp = reserve_temporary();
			__ mov(GET_TEMPORARY(self_tmp), rdi);
			__ mov(call.member->name.c_str(), rsi);
			__ call("snow_get");
			__ mov(rax, GET_TEMPORARY(function_tmp));
			__ mov(GET_TEMPORARY(self_tmp), rdi);
		} else {
			__ comment("closure call");
			__ clear(rdi);         // "self" is NULL for closures
		}
		
		__ mov(GET_TEMPORARY(function_tmp), rsi);
		__ mov(num_args, rdx);
		
		for (size_t i = 0; i < num_args; ++i) {
			const size_t arg_offset = i + 3;
			
			if (arg_offset < num_arg_regs) {
				__ mov(GET_TEMPORARY(args_tmp[i]), *arg_regs[arg_offset]);
			} else {
				// TODO!
			}
			
			free_temporary(args_tmp[i]);
		}
		
		if (self_tmp == function_tmp)
			free_temporary(self_tmp);
		else {
			free_temporary(self_tmp);
			free_temporary(function_tmp);
		}
		
		
		// finally!
		__ clear(rax);
		__ call("snow_call");
	}
	
	void Codegen::compile(ast::Get& get) {
		__ comment("get `" + get.member->name + "'");
		get.self->compile(*this);
		__ mov(rax, rdi);
		__ mov(get.member->name.c_str(), rsi);
		__ call("snow_get");
	}
	
	void Codegen::compile(ast::Set& set) {
		__ comment("set `" + set.member->name + "'");
		set.expression->compile(*this);
		auto tmp = reserve_temporary();
		__ mov(rax, GET_TEMPORARY(tmp));
		set.self->compile(*this);
		__ mov(rax, rdi);
		__ mov(set.member->name.c_str(), rsi);
		__ mov(GET_TEMPORARY(tmp), rdx);
		__ call("snow_set");
		free_temporary(tmp);
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