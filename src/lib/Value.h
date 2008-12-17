#ifndef VALUE_H_BQEU71BP
#define VALUE_H_BQEU71BP

#include <inttypes.h>

namespace snot {

class Object;

typedef void* VALUE;

enum ValueType {
	kObjectType,
	kUndefinedType,
	kBooleanType,
	kIntegerType,
	kNumValueTypes
};

enum SpecialValue {
	kUndefined = 0x0,
	kFalse = 0x2,
	kTrue = 0x4,
	kSpecialValueMax
};

inline bool is_object(VALUE val) { return !((uint64_t)val & 1) && (uint64_t)val > (uint64_t)kSpecialValueMax; }
inline bool is_special(VALUE val) { return (uint64_t)val & 1 && (uint64_t)val < kSpecialValueMax; }
inline bool is_undefined(VALUE val) { return (uint64_t)val == kUndefined; }
inline bool is_boolean(VALUE val) { return (uint64_t)val == kFalse || (uint64_t)val == kTrue; }
inline bool is_true(VALUE val) { return (uint64_t)val == kTrue; }
inline bool is_false(VALUE val) { return (uint64_t)val == kFalse; }
inline bool is_integer(VALUE val) { return (uint64_t)val & 0x01 == 1; }

ValueType value_type(VALUE val);

inline VALUE value(Object* obj) { return (VALUE)obj; }
inline VALUE value(int64_t integer) { return (VALUE)((integer << 1) | 1); }
inline VALUE value(bool b) { return (VALUE)(b ? kTrue : kFalse); }
inline VALUE undefined() { return (VALUE)kUndefined; }


inline int64_t integer(VALUE val) { return ((int64_t)val >> 1) | ((int64_t)val < 0 ? (int64_t)1 << 63 : 0); }
inline bool boolean(VALUE val) { return (int64_t)val == kTrue; }
inline Object* object(VALUE val) { return (Object*)val; }
}

#endif /* end of include guard: VALUE_H_BQEU71BP */
