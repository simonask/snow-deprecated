#ifndef INTERNAL_H_P4ZC50Y5
#define INTERNAL_H_P4ZC50Y5

#include <assert.h>

#ifdef DEBUG
#define ASSERT(expr) { assert(expr); }
#else
#define ASSERT(...) do{}while(0);
#endif

#define TRAP(comment) do{ __asm__("int3\nnop");}while(0);
#define ASSERT_OBJECT(val, T) { ASSERT(object_cast<T>(val) && "expected pointer to object of class " #T); }
#define ASSERT_ARGS(expr) { ASSERT(expr && "wrong number of arguments"); }

#endif /* end of include guard: INTERNAL_H_P4ZC50Y5 */
