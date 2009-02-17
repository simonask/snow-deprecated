#ifndef ASSERT_H_80BKD8NH
#define ASSERT_H_80BKD8NH

#include <assert.h>

#ifdef DEBUG

#define ASSERT_OBJECT(val, T) { assert(object_cast<T>(val) && "expected pointer to object of class " #T); }
#define ASSERT_ARGS(expr) { assert(expr && "wrong number of arguments"); }
#define TRAP() { __asm__("int3"); }

#else

#define ASSERT_OBJECT()
#define ASSERT_ARGS()
#define TRAP()

#endif

#endif /* end of include guard: ASSERT_H_80BKD8NH */
