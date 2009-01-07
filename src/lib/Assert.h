#ifndef ASSERT_H_80BKD8NH
#define ASSERT_H_80BKD8NH

#include <assert.h>

#define assert_object(val, T) { assert(object_cast<T>(val) && "expected pointer to object of class " #T); }
#define assert_args(expr) { assert(expr && "wrong number of arguments"); }

#endif /* end of include guard: ASSERT_H_80BKD8NH */
