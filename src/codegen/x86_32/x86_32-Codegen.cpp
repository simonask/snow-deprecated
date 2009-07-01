#include "x86_32-Codegen.h"
#include "ASTNode.h"
#include "runtime/Runtime.h"
#include "runtime/SnowString.h"
#include "runtime/Function.h"
#include "runtime/Float.h"
#include "base/Internal.h"
#include "base/Util.h"
#include <stdexcept>
#include <vector>
using namespace std;

#define __ m_Asm->
#define e__ entry_asm->

#ifdef DEBUG
#define COMMENT(X) __ comment(X)
#else
#define COMMENT(X)
#endif


namespace snow {
namespace x86_32 {
	// stack_frame->`member'
	#define GET_STACK(member) (Address(ebp, (-(int)sizeof(StackFrame))+(int)offsetof(StackFrame, member)))
	
	// temporaries[id]
	#define GET_TEMPORARY(id) (Address(ebp, (-(int)sizeof(StackFrame))-(sizeof(VALUE)*(id+1))))
	
	// reg[index] (reg must contain a pointer to an array of values)
	#define GET_ARRAY_PTR(reg, index) (Address((reg), index * sizeof(VALUE)))
	
	#define STACK_INPUT_ARG(index) (Address(ebp, (index + 2) * sizeof(VALUE)))
	
	#define STACK_CALL_ARG(index, reg_containing_esp) (Address(reg_containing_esp, index * sizeof(VALUE)))
	
	#ifdef WIN32
	// TODO: stdcall
	static const Register* arg_regs[] = { &edi, &esi, &edx, &ecx };
	static const uintx num_arg_regs = 6;
	#else
	static const Register* arg_regs[] = {};
	static const uintx num_arg_regs = 0;
	#endif

	static const Register* xmm_arg_regs[] = { &xmm0, &xmm1, &xmm2, &xmm3, &xmm4, &xmm5, &xmm6, &xmm7 };
	static const uintx num_xmm_arg_regs = 8;
	
	Codegen::Codegen(ast::FunctionDefinition& def) :
		snow::Codegen(def),
		m_NumLocals(0),
		m_NumStackArguments(3), // reserve minimum 3 args on the stack, for calling runtime functions
		m_NumTemporaries(0) {
		m_LocalMap = gc_new<LocalMap>();
		m_Asm = new x86_32::Assembler;
		m_Return = new Label;
	}
	
	uintx Codegen::reserve_temporary() {
		if (m_FreeTemporaries.size() > 0) {
			uintx t = m_FreeTemporaries.back();
			m_FreeTemporaries.pop_back();
			return t;
		}
		return m_NumTemporaries++;
	}
	
	void Codegen::free_temporary(uintx id) {
		m_FreeTemporaries.push_back(id);
	}
	
	void Codegen::get_local(uintx id, const Register& reg) {
		ASSERT(!m_InGlobalScope);
		__ mov(GET_STACK(locals), reg);
		__ mov(GET_ARRAY_PTR(reg, id), reg);
	}
	
	void Codegen::set_local(const Register& reg, uintx id, const Register& tmp) {
		ASSERT(reg != tmp && "Cannot use source register as temporary storage!");
		ASSERT(!m_InGlobalScope);
		__ mov(GET_STACK(locals), tmp);
		__ mov(reg, GET_ARRAY_PTR(tmp, id));
	}
	
	Ptr<CompiledCode> Codegen::compile(bool in_global_scope) {
		m_InGlobalScope = in_global_scope;

		RefPtr<x86_32::Assembler> entry_asm = new x86_32::Assembler;
		__ subasm(entry_asm);
		
		if (m_Def.arguments.size() > 0) {
			COMMENT("copy arguments to locals");
			__ mov(GET_STACK(args), ecx);
			__ mov(GET_STACK(locals), edx);
			size_t i = 0;
			for each (iter, m_Def.arguments) {
				auto local = m_LocalMap->define_argument((*iter)->name);
				__ mov(GET_ARRAY_PTR(ecx, i), eax);
				__ mov(eax, GET_ARRAY_PTR(edx, local));
				++i;
			}
		}
		
		COMMENT("function body");
		compile(*m_Def.sequence);
		
		uintx return_temporary = reserve_temporary();
		COMMENT("function exit");
		__ bind(m_Return);
		__ mov(eax, GET_TEMPORARY(return_temporary));
		__ call("snow_leave_scope");
		__ mov(GET_TEMPORARY(return_temporary), eax);
		__ leave();
		__ ret();
		
		// Compile the function entry, now that we know all the locals and
		// temporaries.
		{
#ifdef DEBUG
			e__ comment("function entry");
#endif
			int stack_size = sizeof(StackFrame) + sizeof(VALUE)*(m_NumTemporaries + m_NumStackArguments);
			// maintain 16-byte stack alignment
			stack_size += 24 - (stack_size % 16);
			e__ enter(stack_size);
			
			// find temporaries offset on stack
			e__ mov(ebp, esi);
			e__ sub(sizeof(StackFrame) + (m_NumTemporaries)*sizeof(VALUE), esi);
			e__ mov(esi, GET_STACK(temporaries));
			// save number of temporaries
			e__ mov(m_NumTemporaries, GET_STACK(num_temporaries)); // move to register first, for 64-bit operand size.
			
			// debugging pointers
			e__ mov(m_Def.file, GET_STACK(file));
			e__ mov(m_Def.line, GET_STACK(line));
			e__ mov(m_Def.name, GET_STACK(funcname));
			
			// call snow::enter_scope(Scope*, StackFrame*)
			e__ mov(STACK_INPUT_ARG(0), eax);
			e__ mov(esp, ebx);
			e__ mov(eax, STACK_CALL_ARG(0, ebx));
			e__ mov(ebp, esi);
			e__ sub(sizeof(StackFrame), esi);
			e__ mov(esi, STACK_CALL_ARG(1, ebx));
			e__ call("snow_enter_scope");
			e__ mov(nil(), eax);
		}
		
		Ptr<CompiledCode> code = __ compile();
		code->set_local_map(m_LocalMap);
		for each (iter, m_Related) {
			code->add_related(*iter);
		}
		return code;
	}
	
	void Codegen::compile(ast::Literal& literal) {
		COMMENT(std::string("literal: `")  + literal.string + std::string("'"));
		using ast::Literal;
		
		const char* str = literal.string.c_str();
		
		Value val = nil();
		
		switch (literal.type) {
			case Literal::INTEGER_DEC_TYPE:
				val = value((intx)strtol(str, NULL, 10));
				break;
			case Literal::INTEGER_HEX_TYPE:
				val = value((intx)strtol(str, NULL, 16));
				break;
			case Literal::INTEGER_BIN_TYPE:
				val = value((intx)strtol(str, NULL, 2));
				break;
			case Literal::FLOAT_TYPE:
				// TODO: Doubles
				val = malloc_new<Float>(strtof(str, NULL));
				break;
			case Literal::STRING_TYPE:
				val = str[0] ? malloc_new<String>(str) : malloc_new<String>();
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
			case Literal::SYMBOL_TYPE:
				val = (VALUE)Symbol(str);
				break;
		}

		__ mov(val.value(), eax);
	}
	
	void Codegen::compile(ast::Identifier& id) {
		COMMENT(std::string("identifier: `") + value_to_string(id.name) + std::string("'"));
		if (!m_InGlobalScope && m_LocalMap->has_local(id.name)) {
			// It's a local from current scope...
			get_local(m_LocalMap->local(id.name), eax);
		} else {
			// THE PAIN! It's from a parent scope...
			__ mov(esp, ebx);
			__ mov(ebp, edi);
			__ sub(sizeof(StackFrame), edi);
			__ mov(edi, STACK_CALL_ARG(0, ebx));
			__ mov((VALUE)id.name, STACK_CALL_ARG(1, ebx));
			__ mov(id.quiet, STACK_CALL_ARG(2, ebx));
			__ call("snow_get_local");
		}
	}

	void Codegen::compile(ast::Sequence& seq) {
		for each (iter, seq.nodes) {
			(*iter)->compile(*this);
		}
	}

	void Codegen::compile(ast::FunctionDefinition& def) {
		COMMENT("function definition");
		RefPtr<Codegen> codegen = new Codegen(def);
		
		// function name
		std::stringstream ss;
		if (!m_AssignmentFunction)
			ss << "(";
		ss << m_AssignmentName;
		if (!m_AssignmentFunction)
			ss << ")";
		def.name = strdup(ss.str().c_str());

		Ptr<CompiledCode> code = codegen->compile();
		m_Related.push_back(code);
		Ptr<Function> func = malloc_new<Function>(*code);
		__ mov(esp, ebx);
		__ mov(func.value(), STACK_CALL_ARG(0, ebx));
		__ mov(GET_STACK(scope), eax);
		__ mov(eax, STACK_CALL_ARG(1, ebx));
		__ call("snow_set_parent_scope");
		__ mov(func.value(), eax);
	}
	
	void Codegen::compile(ast::Return& ret) {
		COMMENT("return");
		if (ret.expression)
			ret.expression->compile(*this);
		else
			__ mov(nil(), eax);
		__ jmp(m_Return);
	}
	
	void Codegen::compile(ast::LocalAssignment& assign)
	{
		m_AssignmentName = value_to_string(assign.local->name);
		if (assign.expression->is_a<ast::FunctionDefinition>())
			m_AssignmentFunction = true;
		else
			m_AssignmentFunction = false;
		assign.expression->compile(*this);
		COMMENT(std::string("local assignment: ") + m_AssignmentName);

		if (!m_InGlobalScope) {
			uintx l;
			if (m_LocalMap->has_local(assign.local->name))
				l = m_LocalMap->local(assign.local->name);
			else
				l = m_LocalMap->define_local(assign.local->name);
			
			set_local(eax, l);
		} else {
			__ mov(esp, ebx);
			__ mov(ebp, edi);
			__ sub(sizeof(StackFrame), edi);
			__ mov(edi, STACK_CALL_ARG(0, ebx));
			__ mov((VALUE)assign.local->name, STACK_CALL_ARG(1, ebx));
			__ mov(eax, STACK_CALL_ARG(2, ebx));
			__ call("snow_set_local");
		}
		m_AssignmentName = "";
	}

	void Codegen::compile(ast::MemberAssignment& assign)
	{
		COMMENT(std::string("member assignment `") + value_to_string(assign.member->name) + "'");
		m_AssignmentName = std::string(".") + value_to_string(assign.member->name);
		if (assign.expression->is_a<ast::FunctionDefinition>())
			m_AssignmentFunction = true;
		else
			m_AssignmentFunction = false;
		assign.expression->compile(*this);
		auto tmp = reserve_temporary();
		__ mov(eax, GET_TEMPORARY(tmp));
		assign.object->compile(*this);
		__ mov(esp, ebx);
		__ mov(eax, STACK_CALL_ARG(0, ebx));
		__ mov((VALUE)assign.member->name, STACK_CALL_ARG(1, ebx));
		__ mov(GET_TEMPORARY(tmp), eax);
		__ mov(eax, STACK_CALL_ARG(2, ebx));
		__ call("snow_set");
		free_temporary(tmp);
		m_AssignmentName = "";
	}
	
	void Codegen::compile(ast::Member& get) {
		COMMENT(std::string("get `") + value_to_string(get.member->name) + "'");
		get.object->compile(*this);
		__ mov(esp, ebx);
		__ mov(eax, STACK_CALL_ARG(0, ebx));
		__ mov((VALUE)get.member->name, STACK_CALL_ARG(1, ebx));
		__ call("snow_get");
	}

	void Codegen::compile(ast::IfCondition& cond) {
		RefPtr<Label> test_cond = new Label;
		RefPtr<Label> after = new Label;
		
		__ bind(test_cond);
		COMMENT("if cond");
		cond.expression->compile(*this);
		__ mov(esp, ebx);
		__ mov(eax, STACK_CALL_ARG(0, ebx));
		__ call("snow_eval_truth");
		__ cmp(cond.unless, eax);
		__ mov(nil(), eax);
		__ j(CC_EQUAL, after);
		COMMENT("if body");
		cond.if_true->compile(*this);
		__ bind(after);
	}

	void Codegen::compile(ast::IfElseCondition& cond) {
		RefPtr<Label> test_cond = new Label;
		//RefPtr<Label> if_true = new Label;
		RefPtr<Label> if_false = new Label;
		RefPtr<Label> after = new Label;
		
		__ bind(test_cond);
		COMMENT("if-else cond");
		cond.expression->compile(*this);
		__ mov(esp, ebx);
		__ mov(eax, STACK_CALL_ARG(0, ebx));
		__ call("snow_eval_truth");
		__ cmp(cond.unless, eax);
		__ mov(nil(), eax);
		__ j(CC_EQUAL, if_false);
		//__ bind(if_true);
		//COMMENT("if true");
		cond.if_true->compile(*this);
		__ jmp(after);
		__ bind(if_false);
		COMMENT("if false");
		cond.if_false->compile(*this);
		__ bind(after);
	}
	
	void Codegen::compile(ast::IfElseIfElseCondition& cond) {
		RefPtr<ast::Node> trailing_else = cond.if_false;
		RefPtr<ast::IfElseCondition> trunk = new ast::IfElseCondition(cond.expression, cond.if_true, NULL, cond.unless);
		auto deepest = trunk;
		
		while (!cond.else_if.empty()) {
			RefPtr<ast::IfCondition> branch = cond.else_if.front();
			cond.else_if.pop_front();
			
			if (cond.else_if.empty()) {
				if (trailing_else == NULL) {
					deepest->if_false = new ast::IfCondition(branch->expression, branch->if_true);
				} else {
					deepest->if_false = new ast::IfElseCondition(branch->expression, branch->if_true, trailing_else);
				}
			} else {
				deepest->if_false = new ast::IfElseCondition(branch->expression, branch->if_true, NULL);
				deepest = deepest->if_false.cast<ast::IfElseCondition>();
			}
		}
		
		trunk->compile(*this);
	}
	
	void Codegen::generate_store_arguments_for_call(uintx* args_tmp, const RefPtr<ast::Sequence>& args)
	{
		if (!args)
			return;
		size_t i = 0;
		for each (arg_iter, args->nodes) {
			COMMENT(string_printf("argument %d", i));
			(*arg_iter)->compile(*this);
			__ mov(eax, GET_TEMPORARY(args_tmp[i]));
			++i;
		}
	}
	
	void Codegen::generate_refetch_arguments_for_call(uintx* args_tmp, size_t num_args)
	{
		bool esp_in_ebx = false;
		for (size_t i = 0; i < num_args; ++i) {
			const size_t arg_offset = i + 3; // 3 because snow::call takes 3 arguments

			if (arg_offset < num_arg_regs) {
				__ mov(GET_TEMPORARY(args_tmp[i]), *arg_regs[arg_offset]);
			} else {
				const size_t stack_offset = arg_offset - num_arg_regs;
				if (m_NumStackArguments < stack_offset)
					m_NumStackArguments = stack_offset;

				__ mov(GET_TEMPORARY(args_tmp[i]), eax);

				if (!esp_in_ebx) {
					__ mov(esp, ebx);
					esp_in_ebx = true;
				}
				__ mov(eax, STACK_CALL_ARG(stack_offset, ebx));
			}
		}
	}
	
	void Codegen::compile(ast::ExpressionCall& call) {
		auto function_tmp = reserve_temporary();
		COMMENT("local function call");
		
		call.expression->compile(*this);
		__ mov(eax, GET_TEMPORARY(function_tmp));
		
		// evaluate arguments and store temporaries
		size_t num_args = call.arguments ? call.arguments->length() : 0;
		uintx args_tmp[num_args];
		for (size_t i = 0; i < num_args; ++i) {
			args_tmp[i] = reserve_temporary();
		}
		
		generate_store_arguments_for_call(args_tmp, call.arguments);
		
		__ mov(esp, ebx);
		__ mov(0, STACK_CALL_ARG(0, ebx));
		__ mov(GET_TEMPORARY(function_tmp), eax);
		__ mov(eax, STACK_CALL_ARG(1, ebx));
		__ mov(num_args, STACK_CALL_ARG(2, ebx));
	
		generate_refetch_arguments_for_call(args_tmp, num_args);
		
		for (size_t i = 0; i < num_args; ++i) {
			free_temporary(args_tmp[i]);
		}
		
		free_temporary(function_tmp);
		
		__ call("snow_call");
	}
	
	void Codegen::compile(ast::MemberCall& call) {
		auto self_tmp = reserve_temporary();
		COMMENT("member function call");
		
		call.self->compile(*this);
		__ mov(eax, GET_TEMPORARY(self_tmp));
		
		size_t num_args = call.arguments ? call.arguments->length() : 0;
		uintx args_tmp[num_args];
		for (size_t i = 0; i < num_args; ++i) {
			args_tmp[i] = reserve_temporary();
		}
		
		generate_store_arguments_for_call(args_tmp, call.arguments);
		
		__ mov(esp, ebx);
		__ mov(GET_TEMPORARY(self_tmp), edi);
		__ mov(edi, STACK_CALL_ARG(0, ebx));
		__ mov((VALUE)call.member->name, STACK_CALL_ARG(1, ebx));
		__ call("snow_get");
		__ mov(esp, ebx);
		__ mov(eax, STACK_CALL_ARG(1, ebx));
		COMMENT("function for member call");
		__ mov(GET_TEMPORARY(self_tmp), edi);
		__ mov(edi, STACK_CALL_ARG(0, ebx));
		COMMENT("self for member call");
		__ mov(num_args, STACK_CALL_ARG(2, ebx));
		
		generate_refetch_arguments_for_call(args_tmp, num_args);
		
		for (size_t i = 0; i < num_args; ++i) {
			free_temporary(args_tmp[i]);
		}
		
		free_temporary(self_tmp);
		
		__ call("snow_call");
	}

	void Codegen::compile(ast::Loop& loop) {
		RefPtr<Label> test_cond = new Label;
		RefPtr<Label> body = new Label;
		RefPtr<Label> after = new Label;
		
		__ bind(test_cond);
		COMMENT("loop cond");
		loop.expression->compile(*this);
		__ mov(esp, ebx);
		__ mov(eax, STACK_CALL_ARG(0, ebx));
		__ call("snow_eval_truth");
		__ cmp(0, eax);
		__ j(CC_EQUAL, after);
		__ bind(body);
		COMMENT("loop body");
		loop.while_true->compile(*this);
		__ jmp(test_cond);
		__ bind(after);
	}
	
	void Codegen::compile(ast::Self&) {
		__ mov(GET_STACK(self), eax);
	}
	
	void Codegen::compile(ast::It&) {
		__ mov(GET_STACK(it), eax);
	}

	Ptr<CompiledCode> Codegen::compile_proxy(void* function_ptr, const ExternalLibrary::FunctionSignature& signature) {
		RefPtr<x86_32::Assembler> m_Asm = new x86_32::Assembler;
		size_t num_float_args = 0;
		for (uintx i = 0; i < signature.num_args; ++i) {
			if (signature.arg_types[i] == ExternalLibrary::NATIVE_FLOAT && num_float_args < 16) {
				++num_float_args;
			}
		}
		int num_stack_arguments = signature.num_args - num_float_args - num_arg_regs;
		if (num_stack_arguments < 0)
			num_stack_arguments = 0;

		int stack_size = (1 + signature.num_args + num_stack_arguments) * sizeof(void*);
		// maintain 16-byte stack alignment
		stack_size += stack_size % 16;
		__ enter(stack_size);
		
		RefPtr<Label> num_args_matches = new Label;
		__ cmp(signature.num_args, esi);
		__ j(CC_GREATER_EQUAL, num_args_matches);
		__ mov(signature.num_args, edi);
		__ call("snow_external_library_function_num_args_mismatch");
		__ bind(num_args_matches);

		auto args_array_save = Address(ebp, (-(int)signature.num_args - 1) * sizeof(void*));
		__ mov(edx, args_array_save);
		
		for (uintx i = 0; i < signature.num_args; ++i) {
			__ mov(args_array_save, ecx);
			__ mov(Address(ecx, i * sizeof(void*)), edi);
			__ mov(signature.arg_types[i], esi);
			__ call("snow_convert_value_to_native");
			__ mov(eax, Address(ebp, (-(int)signature.num_args + i) * sizeof(void*)));
		}

		unsigned int gpr_taken = 0;
		unsigned int xmm_taken = 0;
		unsigned int stack_taken = 0;

		// Save esp so it can be used for indexing
		__ mov(esp, ebx);

		for (uintx i = 0; i < signature.num_args; ++i) {
			auto current = Address(ebp, (-(int)signature.num_args + i) * sizeof(void*));

			if (signature.arg_types[i] == ExternalLibrary::NATIVE_FLOAT && xmm_taken < num_xmm_arg_regs) {
				__ movd(current, *xmm_arg_regs[xmm_taken++]);
			} else if (signature.arg_types[i] != ExternalLibrary::NATIVE_FLOAT && gpr_taken < num_arg_regs) {
				__ mov(current, *arg_regs[gpr_taken++]);
			} else {
				__ mov(current, eax);
				__ mov(eax, Address(ebx, (stack_taken++) * sizeof(VALUE)));
			}
		}

		__ mov(function_ptr, ebx);
		__ mov(num_float_args, eax);
		__ call(ebx);
		if (signature.return_type == ExternalLibrary::NATIVE_FLOAT)
			__ movd_xmm_gpr(xmm0, edi);
		else
			__ mov(eax, edi);
		__ mov(signature.return_type, esi);
		__ call("snow_convert_native_to_value");
		// return value is in eax already
		__ leave();
		__ ret();

		Ptr<CompiledCode> code = m_Asm->compile();
		return code;
	}
	
	void Codegen::compile(ast::LogicalAnd& statement) {
		RefPtr<Label> is_false = new Label;
		statement.left->compile(*this);
		auto tmp = reserve_temporary();
		__ mov(eax, GET_TEMPORARY(tmp)); // save real return value of left, in case it's false/nil
		__ mov(esp, ebx);
		__ mov(eax, STACK_CALL_ARG(0, ebx));
		__ call("snow_eval_truth");
		__ cmp(0, eax);
		__ mov(GET_TEMPORARY(tmp), eax);
		free_temporary(tmp);
		__ j(CC_EQUAL, is_false);
		statement.right->compile(*this);
		__ bind(is_false);
	}
	
	void Codegen::compile(ast::LogicalOr& statement) {
		RefPtr<Label> is_true = new Label;
		statement.left->compile(*this);
		auto tmp = reserve_temporary();
		__ mov(eax, GET_TEMPORARY(tmp));
		__ mov(esp, ebx);
		__ mov(eax, STACK_CALL_ARG(0, ebx));
		__ call("snow_eval_truth");
		__ cmp(0, eax);
		__ mov(GET_TEMPORARY(tmp), eax);
		free_temporary(tmp);
		__ j(CC_NOT_EQUAL, is_true);
		statement.right->compile(*this);
		__ bind(is_true);
	}
	
	void Codegen::compile(ast::LogicalXor& statement) {
		// TODO: Optimize this
		RefPtr<Label> both_true_or_false = new Label;
		RefPtr<Label> left_was_true = new Label;
		RefPtr<Label> done = new Label;

		auto left_result = reserve_temporary();
		auto right_result = reserve_temporary();
		statement.left->compile(*this);
		__ mov(eax, GET_TEMPORARY(left_result));
		statement.right->compile(*this);
		__ mov(eax, GET_TEMPORARY(right_result));
		__ mov(esp, ebx);
		__ mov(eax, STACK_CALL_ARG(0, ebx));
		__ call("snow_eval_truth");
		auto tmp = reserve_temporary();
		__ mov(eax, GET_TEMPORARY(tmp));
		__ mov(GET_TEMPORARY(left_result), eax);
		__ mov(esp, ebx);
		__ mov(eax, STACK_CALL_ARG(0, ebx));
		__ call("snow_eval_truth");
		__ mov(GET_TEMPORARY(tmp), ecx);
		
		__ cmp(eax, ecx);
		// they were both either true or false, so return false
		__ j(CC_EQUAL, both_true_or_false);
		
		// find out which was true
		__ cmp(0, eax);
		__ j(CC_NOT_EQUAL, left_was_true);
		__ mov(GET_TEMPORARY(right_result), eax);
		__ jmp(done);
		
		__ bind(left_was_true);
		__ mov(GET_TEMPORARY(left_result), eax);
		__ jmp(done);
		
		__ bind(both_true_or_false);
		__ mov(value(false), eax);
		
		__ bind(done);
	}
	
	void Codegen::compile(ast::LogicalNot& statement) {
		RefPtr<Label> was_true = new Label;
		RefPtr<Label> done = new Label;
		
		statement.expr->compile(*this);
		__ mov(esp, ebx);
		__ mov(eax, STACK_CALL_ARG(0, ebx));
		__ call("snow_eval_truth");
		__ cmp(0, eax);

		__ j(CC_NOT_EQUAL, was_true);
		
		__ mov(value(true), eax);
		__ jmp(done);
		
		__ bind(was_true);
		__ mov(value(false), eax);

		__ bind(done);
	}
}
}
