#ifndef THINOBJECT_H_JLSFLZC3
#define THINOBJECT_H_JLSFLZC3

#include "Basic.h"
#include "IGarbage.h"
#include "IObject.h"
#include "Handle.h"
#include "Value.h"
#include "MemoryManager.h"
#include "Garbage.h"

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
	class ThinObject : public IObject, public IGarbage {
	protected:
		Object* m_Prototype;
		volatile bool m_Frozen;
		
		explicit ThinObject(const Handle<Object>& prototype = NULL) : m_Prototype(prototype), m_Frozen(false) {}
		ThinObject(const ThinObject& other) : m_Prototype(other.m_Prototype), m_Frozen(false) {}
		
		virtual void gc_mark() { /*Garbage::gc_mark(); Garbage::mark(m_Prototype);*/ }
	public:
		virtual ~ThinObject() {}
		
		virtual VALUE va_call(VALUE self, uint64_t, va_list&);
		
		bool is_frozen() const { return m_Frozen; }
		virtual void freeze() { m_Frozen = true; }
		virtual void unfreeze() { m_Frozen = false; }
		
		virtual VALUE get(const std::string& name) const;
		virtual VALUE set(const std::string& name, VALUE);
		
		Object* prototype() const;
		void set_prototype(Object* proto) { m_Prototype = proto; }
		
		void* operator new(size_t sz, AllocatorType type = kGarbage) { return ::operator new(sz, type); };
	};
}

#endif /* end of include guard: THINOBJECT_H_JLSFLZC3 */
