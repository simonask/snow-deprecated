#ifndef FUNCTIONTYPES_H_PV9PUENG
#define FUNCTIONTYPES_H_PV9PUENG

#include "runtime/Handle.h"

namespace snow {
	class Scope;
	struct Arguments;
	
	typedef VALUE(*FunctionPtr)(Scope*);
	typedef Value(*NativeFunctionPtr)(const Value& self, const Arguments&);
}

#endif /* end of include guard: FUNCTIONTYPES_H_PV9PUENG */
