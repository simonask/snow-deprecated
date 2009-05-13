#ifndef BASIC_H_T0Q2U1RW
#define BASIC_H_T0Q2U1RW

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

namespace snow {
	typedef unsigned char byte;

	// Used for template magic
	struct TrueType {};
	struct FalseType {};
}

#define CAPI extern "C"

#include "Iterate.h"
#include "Util.h"
#include "RefPtr.h"


#endif /* end of include guard: BASIC_H_T0Q2U1RW */
