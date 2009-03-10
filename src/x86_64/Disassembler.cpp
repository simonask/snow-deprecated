#include "Disassembler.h"
#include "Assembler.h"

#include <sstream>
#include <iostream>

namespace snow {
namespace x86_64 {
	
	struct RegisterMnemonic {
		const Register* reg;
		const char* mnemonic;
	};
	
	static const RegisterMnemonic registers[] = {
		{&rax, "rax"},
		{&rcx, "rcx"},
		{&rdx, "rdx"},
		{&rbx, "rbx"},
		{&rsp, "rsp"},
		{&rbp, "rbp"},
		{&rsi, "rsi"},
		{&rdi, "rdi"},
		{&r8, "r8"},
		{&r9, "r9"},
		{&r10, "r10"},
		{&r11, "r11"},
		{&r12, "r12"},
		{&r13, "r13"},
		{&r14, "r14"},
		{&r15, "r15"},
		{NULL, NULL}
	};
	
	enum OperandType {
		OO_UNSET = 0,
		OO_NO_OPERANDS = 0,
		OO_BAKED = 1,
		OO_REG_OPER = 1 << 1,
		OO_OPER_REG = 1 << 2,
		OO_SINGLE = 1 << 3,
		OO_REG_EXTENSION = 1 << 4
	};
	
	struct InstructionType {
		int opcode;
		const char* mnemonic;
		int operand_type;
		int immediate_size;
	};
	
	struct RegExtensionInstructionType {
		int opcode;
		int reg;
		const char* mnemonic;
		int operand_type;
		int immediate_size;
	};
	
	struct Instruction {
		const byte* offset;
		const byte* next;
		
		byte rex;

		InstructionType type;
		RegExtensionInstructionType reg_type;

		int opcode;
		const char* mnemonic;
		int operand_type;
		Assembler::RM_MODE mod;	// is rm register, address, address with displacement?
		int reg;	// register or opcode-ext
		int rm;		// register or address
		int32_t displacement;
		
		SIB::SCALE_MODE scale;
		int base;	// register
		int index;	// register
		
		size_t immediate_size;
		int64_t immediate;
	};
	
	static InstructionType instructions[] = {
		{0x01, "add", OO_REG_OPER, 0},
		{0x03, "add", OO_OPER_REG, 0},
		{0x21, "and", OO_REG_OPER, 0},
		{0x23, "and", OO_OPER_REG, 0},
		{0xe8, "call", OO_UNSET, 4},
		{0x40, "cmov", OO_BAKED | OO_OPER_REG, 0},
		{0x38, "cmpb", OO_OPER_REG, 1},
		{0x39, "cmp", OO_OPER_REG, 4},
		{0x3a, "cmpb", OO_REG_OPER, 0},
		{0x3b, "cmp", OO_REG_OPER, 0},
		{0xc8, "enter", OO_UNSET, 3},
		{0x69, "imul", OO_OPER_REG, 4},
		{0xe9, "jmp", OO_UNSET, 4},
		{0x8d, "lea", OO_OPER_REG, 0},
		{0x89, "mov", OO_REG_OPER, 0},
		{0x8b, "mov", OO_OPER_REG, 0},
		{0xb8, "mov", OO_BAKED, 8},
		{0x09, "or", OO_REG_OPER, 0},
		{0x0a, "or", OO_OPER_REG, 0},
		{0x58, "pop", OO_BAKED, 0},
		{0x8f, "pop", OO_SINGLE, 0},
		{0x50, "push", OO_BAKED, 0},
		{0x68, "push", OO_UNSET, 4},
		{0x29, "sub", OO_REG_OPER, 0},
		{0x2b, "sub", OO_OPER_REG, 0},
		{0x85, "test", OO_REG_OPER, 0},
		{0x87, "xchg", OO_REG_OPER, 0},
		{0x31, "xor", OO_REG_OPER, 0},
		{0x33, "xor", OO_OPER_REG, 0},
		{0xcd, "int", OO_UNSET, 1},
		{0xcc, "int3", OO_UNSET, 0},
		{0x9f, "lahf", OO_UNSET, 0},
		{0xc9, "leave", OO_UNSET, 0},
		{0x90, "nop", OO_UNSET, 0},
		{0x9c, "pushf", OO_UNSET, 0},
		{0xc3, "ret", OO_UNSET, 0},
		{0xcb, "ret_far", OO_UNSET, 0},
		{0x9e, "sahf", OO_UNSET, 0},
		{0xf9, "stc", OO_UNSET, 0},
		{0xfd, "std", OO_UNSET, 0},
		{0x80, "", OO_REG_EXTENSION, 0},
		{0x81, "", OO_REG_EXTENSION, 0},
		{0xc7, "", OO_REG_EXTENSION, 0},
		{0xf7, "", OO_REG_EXTENSION, 0},
		{0xff, "", OO_REG_EXTENSION, 0},
		{-1, NULL, OO_UNSET, -1}
	};
	
	static InstructionType two_byte_instructions[] = {
		{0xc8, "bswap", OO_BAKED, 0},
		{0xaf, "imul", OO_OPER_REG, 0},
		{0x80, "j", OO_BAKED, 4},
		{0xa2, "cpuid", OO_UNSET, 0},
		{0x90, "pause", OO_UNSET, 0},
		
		{-1, NULL, OO_UNSET, -1}
	};
	
	static InstructionType three_byte_instructions[] = {
		{0xbd, "lzcnt", OO_OPER_REG, 0},
		{0xb8, "popcnt", OO_OPER_REG, 0},
		{0xe8, "lfence", OO_UNSET, 0},
		{0xf0, "mfence", OO_UNSET, 0},
		
		{-1, NULL, OO_UNSET}
	};
	
	static RegExtensionInstructionType reg_extension_instructions[] = {
		{0x81, 0, "add", OO_SINGLE, 4},
		{0x81, 4, "and", OO_SINGLE, 4},
		{0x80, 7, "cmpb", OO_SINGLE, 4},
		{0x81, 7, "cmpb", OO_SINGLE, 4},
		{0xff, 2, "call", OO_SINGLE, 0},
		{0xfe, 1, "decb", OO_SINGLE, 0},
		{0xff, 1, "dec", OO_SINGLE, 0},
		{0xf7, 6, "div", OO_SINGLE, 0},
		{0xf7, 7, "idiv", OO_UNSET, 0},
		{0xfe, 0, "incb", OO_SINGLE, 0},
		{0xff, 0, "inc", OO_SINGLE, 0},
		{0xff, 4, "jmp", OO_SINGLE, 0},
		{0xff, 3, "jmp_far", OO_SINGLE, 0},
		{0xc7, 0, "mov", OO_SINGLE, 4},
		{0xf7, 4, "mul", OO_SINGLE, 0},
		{0xf7, 3, "neg", OO_SINGLE, 0},
		{0xf7, 2, "not", OO_SINGLE, 0},
		{0x81, 1, "or", OO_SINGLE, 4},
		{0xff, 6, "push", OO_SINGLE, 0},
		{0x81, 5, "sub", OO_SINGLE, 4},
		{0xf7, 0, "test", OO_SINGLE, 4},
		{0x81, 6, "xor", OO_SINGLE, 4},
		
		{-1, -1, NULL, OO_UNSET, -1}
	};
	
	static bool find_instruction_type(Instruction& instr, const InstructionType* types, byte opcode) {
		const InstructionType* iterator = types;
		while (iterator->opcode >= 0) {
			if (opcode == iterator->opcode || ((iterator->operand_type & OO_BAKED) && ((opcode & 0xf8) == iterator->opcode))) {
				instr.type = *iterator;
				instr.opcode = opcode;
				instr.mnemonic = iterator->mnemonic;
				instr.operand_type = iterator->operand_type;
				instr.immediate_size = iterator->immediate_size;
				return true;
			}
			
			++iterator;
		}
		return false;
	}
	
	static bool find_reg_extension_instruction_type(Instruction& instr) {
		const RegExtensionInstructionType* iterator = reg_extension_instructions;
		while (iterator->mnemonic) {
			if (instr.type.opcode == iterator->opcode && instr.reg == iterator->reg) {
				instr.reg_type = *iterator;
				instr.mnemonic = iterator->mnemonic;
				instr.operand_type |= iterator->operand_type;
				instr.immediate_size = iterator->immediate_size;
				return true;
			}
			++iterator;
		}
		return false;
	}
	
	static const byte* parse_opcode_3(Instruction& instr, const byte* raw) {
		byte a = *raw++;
		if (find_instruction_type(instr, three_byte_instructions, a)) {
			// TODO
		} else {
			error("Unknown 3-byte opcode at 0x%llx: 0x%x", raw-3, a);
			TRAP();
		}
		return raw;
	}
	
	static const byte* parse_opcode_2(Instruction& instr, const byte* raw) {
		byte a = *raw++;
		if (a == 0xae || a == 0xf3) {
			return parse_opcode_3(instr, raw);
		} else {
			if (find_instruction_type(instr, two_byte_instructions, a)) {
				// TODO
			} else {
				error("Unknown 2-byte opcode at 0x%llx: 0x%x", raw-2, a);
				TRAP();
			}
		}
		return raw;
	}
	
	static const byte* parse_opcode_1(Instruction& instr, const byte* raw) {
		byte a = *raw++;
		if (a == 0x0f) {
			return parse_opcode_2(instr, raw);
		} else {
			if (find_instruction_type(instr, instructions, a)) {
				// TODO
			} else {
				error("Unknown 1-byte opcode at 0x%llx: 0x%x", raw-1, a);
				TRAP();
			}
		}
		return raw;
	}
	
	
	static const byte* parse_rex(byte& rex, const byte* raw) {
		rex = Assembler::NO_REX;
		byte flags = *raw;
		if ((flags & 0xf0) == 0x40) {
			++raw;
			rex = flags & 0x0f;
		}
		return raw;
	}
	
	static const byte* parse_operands(Instruction& instr, const byte* raw) {
		byte modrm = *raw++;
		instr.rm = modrm & 0x7;
		instr.mod = static_cast<Assembler::RM_MODE>(modrm >> 6);
		instr.reg = (modrm & 0x38) >> 3;
		
		if (instr.type.operand_type & OO_REG_EXTENSION) {
			if (!find_reg_extension_instruction_type(instr)) {
				error("Unknown extended opcode 0x%x/%d!", instr.type.opcode, instr.reg);
				TRAP();
			}
		}
		
		if (instr.rm == 4) {
			// SIB!
			byte sib = *raw++;
			instr.scale = static_cast<SIB::SCALE_MODE>(sib >> 6);
			instr.base = sib & 0x07;
			instr.index = (sib & 0x38) >> 3;
		}
		
		switch (instr.mod) {
			case Assembler::RM_ADDRESS_DISP8:
				// for sign
				instr.displacement = *(const char*)raw;
				raw++;
				break;
			case Assembler::RM_ADDRESS_DISP32:
				// Only works because we're little-endian...
				memcpy(&instr.displacement, raw, 4);
				//instr.displacement = *reinterpret_cast<const int32_t*>(raw);
				raw += 4;
				break;
			default:
				break;
		}
		
		return raw;
	}
	
	static const byte* parse_immediate(Instruction& instr, const byte* raw) {
		if (instr.immediate_size > 0) {
			if (instr.immediate_size == 4) {
				int imm;
				memcpy(&imm, raw, 4);
				instr.immediate = imm;
			} else
				memcpy(&instr.immediate, raw, instr.immediate_size);
			raw += instr.immediate_size;
		}
		return raw;
	}
	
	static const byte* parse_instr(Instruction& instr, const byte* raw) {
		instr.offset = raw;
		
		raw = parse_rex(instr.rex, raw);
		
		raw = parse_opcode_1(instr, raw);
		
		if (instr.type.operand_type & (OO_UNSET | OO_REG_OPER | OO_OPER_REG | OO_REG_EXTENSION)) {
			raw = parse_operands(instr, raw);
		}
		
		raw = parse_immediate(instr, raw);
		
		instr.next = raw;
		
		return raw;
	}
	
	static std::string get_reg_name(byte reg, bool wide) {
		const RegisterMnemonic* iterator = registers;
		while (iterator->reg) {
			if (iterator->reg->code() == reg && iterator->reg->extended() == wide)
				return std::string("%") + std::string(iterator->mnemonic);
			++iterator;
		}
		error("Register not found: 0x%x/%d", reg, wide);
		TRAP();
		return "";
	}
	
	static std::string get_rm_name(Instruction& instr) {
		std::string reg = get_reg_name(instr.rm, instr.rex & Assembler::REX_EXTEND_RM);
		std::stringstream ss;
		switch (instr.mod) {
			case Assembler::RM_ADDRESS:
				ss << "(" << reg << ")";
			break;
			case Assembler::RM_ADDRESS_DISP8:
			case Assembler::RM_ADDRESS_DISP32:
				ss << instr.displacement << "(" << reg << ")";
			break;
			default:
				ss << reg;
		}
		return ss.str();
	}
	
	static std::string get_cc_name(byte cc) {
		switch (cc) {
			case CC_OVERFLOW:
				return "o";
			case CC_NOT_OVERFLOW:
				return "no";
			case CC_BELOW:
			return "b";
			case CC_NOT_BELOW:
			return "nb";
			case CC_EQUAL:
			return "e";
			case CC_NOT_EQUAL:
			return "ne";
			case CC_NOT_ABOVE:
			return "na";
			case CC_ABOVE:
			return "a";
			case CC_SIGN:
			return "s";
			case CC_NOT_SIGN:
			return "ns";
			// XXX: Parity mnemonics are probably wrong, but nobody bothered to look them up.
			case CC_PARITY_EVEN:
			return "pe";
			case CC_PARITY_ODD:
			return "po";
			case CC_LESS:
			return "l";
			case CC_GREATER_EQUAL:
			return "ge";
			case CC_LESS_EQUAL:
			return "le";
			case CC_GREATER:
			return "g";
		}
		error("Unknown condition 0x%x!", cc);
		TRAP();
		return "";
	}
	
	std::string Disassembler::disassemble(const CompiledCode& code, const SymbolTable& table, bool include_offsets) {
		const byte* raw = code.code();
		const byte* end = &raw[code.size()];
		
		std::stringstream ss;
		
		while (raw < end) {
			Instruction instr;
			raw = parse_instr(instr, raw);
			
			if (include_offsets) {
				ss << std::hex << (void*)instr.offset << ":    ";
			}
			
			ss << instr.mnemonic;
			
			if (instr.type.opcode == 0x80) {  // jcc
				ss << get_cc_name(instr.opcode & 0x7);
			}
			
			std::vector<std::string> operand_strings;
			
			if (instr.immediate_size > 0) {
				std::stringstream tmp;
				switch (instr.type.opcode) {
					case 0xc8:  // enter
					{
						int stacksize = instr.immediate & 0xffff;
						int level = (instr.immediate & 0xff0000) >> 16;
						tmp << "$0x" << std::hex << stacksize << ", $0x" << std::hex << level;
						break;
					}
					case 0xe8:  // call
					{
						// call addresses are relative, so find absolute address.
						ptrdiff_t offset = instr.immediate;
						const void* ptr = instr.next + offset;
						tmp << "$" << std::hex << ptr;

						for each (iter, table) {
							if (iter->second.address() == ptr) {
								tmp << " <" << iter->first << ">";
								break;
							}
						}
						break;
					}
					case 0x80:  // jcc
					case 0xe9:  // jmp
					{
						ptrdiff_t offset = instr.immediate;
						const void* ptr = instr.next + offset;
						tmp << "$" << std::hex << ptr;
						break;
					}
					default:
						tmp << "$0x" << std::hex << instr.immediate;
					break;
				}
				operand_strings.push_back(tmp.str());
			}
			
			if (instr.operand_type & OO_BAKED) {
				int reg = instr.opcode & 0x7;
				if (instr.type.opcode != 0x80) // jcc
					operand_strings.push_back(get_reg_name(reg, instr.rex & Assembler::REX_EXTEND_OPCODE));
			} else if (instr.operand_type & (OO_SINGLE | OO_OPER_REG | OO_REG_OPER)) {
				// TODO: SIB
				std::string rm = get_rm_name(instr);
				if (instr.operand_type & OO_SINGLE) {
					operand_strings.push_back(rm);
				} else {
					std::string reg = get_reg_name(instr.reg, instr.rex & Assembler::REX_EXTEND_REG);
					if (instr.operand_type & OO_OPER_REG) {
						operand_strings.push_back(rm);
						operand_strings.push_back(reg);
					} else {
						operand_strings.push_back(reg);
						operand_strings.push_back(rm);
					}
				}
			}
			
			size_t i = 0;
			for each (iter, operand_strings) {
				ss << " ";
				ss << *iter;
				if (i != operand_strings.size()-1)
					ss << ",";
				++i;
			}
			
			ss << std::endl;
		}
		
		return ss.str();
	}
}
}