#ifndef VALUE_H_BQEU71BP
#define VALUE_H_BQEU71BP

#include "base/Basic.h"

namespace snow {

class IObject;
class Object;
class Garbage;
class Scope;

typedef void* VALUE;

typedef VALUE(*FunctionPtr)(Scope*);
typedef VALUE(*NativeFunctionPtr)(VALUE self, uintx num_args, VALUE* args);

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

inline bool is_integer(VALUE val) { return (intx)val & 0x1; }
inline bool is_object(VALUE val) { return val && ((intx)val & kTypeMask) == kObjectType; }
inline bool is_true(VALUE val) { return (intx)val == kTrue; }
inline bool is_false(VALUE val) { return (intx)val == kFalse; }
inline bool is_boolean(VALUE val) { return is_true(val) || is_false(val); }
inline bool is_nil(VALUE val) { return (intx)val == kNil; }
inline bool is_symbol(VALUE val) { return ((intx)val & kTypeMask) == kSymbolType; }
inline bool is_float(VALUE val) { return ((intx)val & kTypeMask) == kFloatType; }
inline bool is_numeric(VALUE val) { return is_integer(val) || is_float(val); }

inline VALUE value(IObject* obj) { return static_cast<VALUE>(obj); }
inline VALUE value(intx integer) { return (VALUE)((integer << 1) | 1); }
inline VALUE value(long long int integer) { return value((intx)integer); }
inline VALUE value(bool b) { return (VALUE)(b ? kTrue : kFalse); }
inline VALUE value(float f) { return (VALUE)(((uintx)*reinterpret_cast<uint32_t*>(&f) << 16) | kFloatType); }
// needed to avoid implicit conversion of VALUE to bool in cases like value(nil())
inline VALUE value(VALUE val) { return val; }

inline VALUE nil() { return (VALUE)kNil; }

inline intx integer(VALUE val) {
	#ifdef ARCH_IS_64_BIT
	return ((intx)val >> 1) | ((intx)val < 0 ? (intx)1 << 63 : 0);
	#else
	return ((intx)val >> 1) | ((intx)val < 0 ? (intx)1 << 31 : 0);
	#endif
}

inline bool boolean(VALUE val) { return val && (intx)val != kFalse && (intx)val != kNil; }

#ifdef ARCH_IS_64_BIT
// Use immediate inline floats if our pointer type has room for them.
inline float floatnum(VALUE val) { uint32_t d = (uint32_t)((uintx)val >> 16); return *reinterpret_cast<float*>(&d); }
#else
inline float floatnum(VALUE val) { /* TODO */ return 0.f; }
#endif

inline bool eval_truth(VALUE val) { return boolean(val) || is_object(val) || is_integer(val); }

template <class T = IObject>
inline T* object_cast(VALUE val) {
	if (!is_object(val))
		return 0;
	// XXX: Assumes that Garbage is the base of _all_ objects.
	return dynamic_cast<T*>((IObject*)val);
}
}

#endif /* end of include guard: VALUE_H_BQEU71BP */
