#ifndef FUNCTION_H_W6S07BKS
#define FUNCTION_H_W6S07BKS

#include "IncrementalAlloc.h"

namespace snot {
	typedef void(*FunctionPointer)();
	
	class Function {
	private:
		FunctionPointer m_Code;
		Function() : m_Code(NULL) {}
	public:
		Function(const Function& other) : m_Code(other.m_Code) {}
		Function(FunctionPointer external) : m_Code(external) {}
		
		FunctionPointer ptr() const { return m_Code; }
		
		static Function make(size_t len) {
			Function f;
			f.m_Code = (FunctionPointer)incremental_alloc(len);
			return f;
		}
	};
}

#endif /* end of include guard: FUNCTION_H_W6S07BKS */
