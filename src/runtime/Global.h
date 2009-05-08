#ifndef GLOBAL_H_OR4WZ1AZ
#define GLOBAL_H_OR4WZ1AZ

#include "Value.h"

namespace snow {
	class Scope;
	
	class Global {
	private:
		Global() {}
	public:
		static void define_globals(Scope& scope);
	};
}

#endif /* end of include guard: GLOBAL_H_OR4WZ1AZ */
