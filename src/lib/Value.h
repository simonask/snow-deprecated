#ifndef VALUE_H_BQEU71BP
#define VALUE_H_BQEU71BP

#include <inttypes.h>

namespace snow {

class Object;
class Garbage;
class Scope;

typedef void* VALUE;

typedef VALUE(*FunctionPtr)(Scope*);
typedef VALUE(*NativeFunctionPtr)(VALUE self, uint64_t num_args, VALUE* args);

enum ValueType {
	kObjectType = 0x0,
	kIntegerType = 0x1,
	kNil = 0x2,
	kFalse = 0x4,
	kTrue = 0x6,
	kSymbolType = 0xa,
	kFloatType = 0xe,
	kSpecialTypeMask = 0xe,
	
	kTypeMask = 0xf
};

inline bool is_integer(VALUE val) { return (int64_t)val & 0x1; }
inline bool is_object(VALUE val) { return val && ((int64_t)val & kTypeMask) == kObjectType; }
inline bool is_true(VALUE val) { return (int64_t)val == kTrue; }
inline bool is_false(VALUE val) { return (int64_t)val == kFalse; }
inline bool is_boolean(VALUE val) { return is_true(val) || is_false(val); }
inline bool is_nil(VALUE val) { return (int64_t)val == kNil; }
inline bool is_symbol(VALUE val) { return ((int64_t)val & kTypeMask) == kSymbolType; }
inline bool is_float(VALUE val) { return ((int64_t)val & kTypeMask) == kFloatType; }

inline VALUE value(Object* obj) { return (VALUE)obj; }
inline VALUE value(int64_t integer) { return (VALUE)((integer << 1) | 1); }
inline VALUE value(bool b) { return (VALUE)(b ? kTrue : kFalse); }
inline VALUE value(float f) { return (VALUE)(((uint64_t)*reinterpret_cast<uint32_t*>(&f) << 16) | kFloatType); }

inline VALUE nil() { return (VALUE)kNil; }

inline int64_t integer(VALUE val) { return ((int64_t)val >> 1) | ((int64_t)val < 0 ? (int64_t)1 << 63 : 0); }
inline bool boolean(VALUE val) { return val && (int64_t)val != kFalse; }
inline float floatnum(VALUE val) { uint32_t d = (uint32_t)((uint64_t)val >> 16); return *reinterpret_cast<float*>(&d); }

inline bool eval_truth(VALUE val) { return boolean(val) || is_object(val) || is_integer(val); }

template <class T = Object>
inline T* object_cast(VALUE val) {
	if (!is_object(val))
		return 0;
	// XXX: Assumes that Garbage is the base of _all_ objects.
	return dynamic_cast<T*>((Garbage*)val);
}
}

#endif /* end of include guard: VALUE_H_BQEU71BP */
