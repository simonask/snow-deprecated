#ifndef VALUE_H_BQEU71BP
#define VALUE_H_BQEU71BP

#include <inttypes.h>

namespace snot {

class Object;

typedef void* VALUE;

enum ValueType {
	kObjectType = 0,
	kEvenIntegerType = 0x1,
	kOddIntegerType = 0x3,
	kSpecialType = 0x2,
	kTypeMask = 0x3
};

enum SpecialValue {
	kTrue = 0x2,
	kFalse = 0x6,
	kUndefined = 0xe,
	kSpecialValueMax
};

inline ValueType value_type(VALUE val) { return (ValueType)((uint64_t)val & kTypeMask); }
inline SpecialValue special_value(VALUE val) { return (SpecialValue)(uint64_t)val; }

inline bool is_integer(VALUE val) { return value_type(val) == kEvenIntegerType || value_type(val) == kOddIntegerType; }
inline bool is_object(VALUE val) { return value_type(val) == kObjectType; }
inline bool is_special(VALUE val) { return value_type(val) == kSpecialType; }
inline bool is_true(VALUE val) { return (int64_t)val == kTrue; }
inline bool is_false(VALUE val) { return (int64_t)val == kFalse; }
inline bool is_boolean(VALUE val) { return is_true(val) || is_false(val); }
inline bool is_undefined(VALUE val) { return (int64_t)val == kUndefined; }

inline VALUE value(Object* obj) { return (VALUE)obj; }
inline VALUE value(int64_t integer) { return (VALUE)((integer << 1) + 1); }
inline VALUE value(bool b) { return (VALUE)(b ? kTrue : kFalse); }
inline VALUE undefined() { return (VALUE)kUndefined; }

inline int64_t integer(VALUE val) { return ((int64_t)val >> 1) | ((int64_t)val < 0 ? (int64_t)1 << 63 : 0); }
inline bool boolean(VALUE val) { return (int64_t)val == kTrue; }
inline Object* object(VALUE val) { return (Object*)val; }

inline bool eval_truth(VALUE val) { return boolean(val) || is_object(val) || is_integer(val); }
}

#endif /* end of include guard: VALUE_H_BQEU71BP */
