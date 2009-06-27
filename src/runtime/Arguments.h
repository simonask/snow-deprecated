#ifndef ARGUMENTS_H_OO50I59H
#define ARGUMENTS_H_OO50I59H

#include "runtime/Value.h"

namespace snow {
	struct Arguments {
		VALUE* data;
		uintx size;
		Arguments() : data(NULL), size(0) {}
	};
}

#endif /* end of include guard: ARGUMENTS_H_OO50I59H */
