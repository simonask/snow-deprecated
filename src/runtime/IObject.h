#ifndef IOBJECT_H_1VTRMSXU
#define IOBJECT_H_1VTRMSXU

#include "base/Basic.h"
#include "gc/IGarbage.h"
#include "gc/Garbage.h"
#include "runtime/Value.h"
#include "runtime/Symbol.h"
#include <stdarg.h>

namespace snow {
	class Object; 
	struct Arguments;
	
	class IObject : public IGarbage {
	public:
		virtual Ptr<Object> prototype() const = 0;
		virtual Ptr<IObject> copy() const = 0;
		
		virtual uintx id() const = 0;
		virtual bool has_member(Symbol name) const = 0;
		virtual Value get_raw(Symbol name) const = 0;
		virtual Value set_raw(Symbol name, const Value&) = 0;

		virtual Value get(const Value& self, Symbol name) const = 0;
		virtual Value set(const Value& self, Symbol name, const Value& val) = 0;
		
		virtual Value call(const Value& self, const Arguments&) = 0;
	};
	
	template <class T = IObject>
	inline Ptr<T> object_cast(VALUE val) {
		if (!is_object(val))
			return 0;
		// XXX: Assumes that Garbage is the base of _all_ objects.
		return dynamic_cast<T*>((IObject*)val);
	}

	template <class T = IObject>
	inline Ptr<T> object_cast(const Value& val) {
		return object_cast<T>(val.value());
	}
}

#endif /* end of include guard: IOBJECT_H_1VTRMSXU */
