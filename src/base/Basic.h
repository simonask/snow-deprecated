#ifndef BASIC_H_T0Q2U1RW
#define BASIC_H_T0Q2U1RW

#ifdef ARCH_x86_64
#define ARCH_IS_64_BIT 1
#endif

#ifdef ARCH_x86_32
#define ARCH_IS_32_BIT 1
#endif

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <type_traits>

#include <omp.h>

namespace snow {
	
	#ifdef ARCH_IS_64_BIT

	typedef intx intx;
	typedef uintx uintx;

	#else

	typedef int32_t intx;
	typedef uint32_t uintx;

	#endif
	
	typedef unsigned char byte;
}

#define CAPI extern "C"

#include "Iterate.h"
#include "Util.h"
#include "RefPtr.h"


#endif /* end of include guard: BASIC_H_T0Q2U1RW */
