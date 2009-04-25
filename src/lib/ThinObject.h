#ifndef THINOBJECT_H_JLSFLZC3
#define THINOBJECT_H_JLSFLZC3

#include "Basic.h"
#include "IObject.h"
#include "Handle.h"
#include "Value.h"
#include "Symbol.h"

namespace snow {
	class Object;
	
	/**
	 * ThinObject is a slimmer version of Object, that can't have member variables.
	 * Why is this useful? Well, it isn't. But you can derive from this class to define
	 * value-types (math classes such as Complex, Vector, Matrix come to mind).
	 * 
	 * Generally, ThinObject should be used whenever it doesn't make sense for
	 * your object to have instance-specific member variables.
	 * 
	 * Performance benefit comes from not having to allocate an Object::Members
	 * structure for each instance
	 */
	class ThinObject : public IObject {
	protected:
		Object* m_Prototype;
		volatile bool m_Frozen;
		
		explicit ThinObject(Object* prototype = NULL) : m_Prototype(prototype), m_Frozen(false) {}
		ThinObject(const ThinObject& other) : m_Prototype(other.m_Prototype), m_Frozen(false) {}
		
		//virtual void gc_func(GCFunc func) { func(m_Prototype); }
		GC_ROOTS { GC_ROOT(m_Prototype); }
		bool gc_try_lock() { return true; }
		void gc_unlock() {}
	public:
		virtual ~ThinObject() {}
		
		virtual VALUE va_call(VALUE self, uint64_t, va_list&);
		
		bool is_frozen() const { return m_Frozen; }
		virtual void freeze() { m_Frozen = true; }
		virtual void unfreeze() { m_Frozen = false; }
		
		virtual bool has_member(VALUE) const { return false; }
		virtual VALUE get(VALUE name) const;
		virtual VALUE set(VALUE name, VALUE);
		virtual VALUE get_by_string(const char* s) const { return get(symbol(s)); }
		virtual VALUE set_by_string(const char* s, VALUE val) { return set(symbol(s), val); }
		
		Object* prototype() const;
		void set_prototype(Object* proto) { m_Prototype = proto; }
	};
}

#endif /* end of include guard: THINOBJECT_H_JLSFLZC3 */
