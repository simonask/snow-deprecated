#ifndef DISASSEMBLER_H_X13OZ5CG
#define DISASSEMBLER_H_X13OZ5CG

#include "CompiledCode.h"

namespace snow {
namespace x86_64 {
	class Disassembler {
	public:
		static std::string disassemble(const CompiledCode& code, bool include_offsets = true);
	};
}
}

#endif /* end of include guard: DISASSEMBLER_H_X13OZ5CG */
