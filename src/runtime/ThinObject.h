#ifndef THINOBJECT_H_JLSFLZC3
#define THINOBJECT_H_JLSFLZC3

#include "base/Basic.h"
#include "IObject.h"
#include "Handle.h"
#include "Value.h"
#include "Symbol.h"

namespace snow {
	class Object;
	
	Ptr<Object> object_prototype();
	
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
		Ptr<Object> m_Prototype;
		struct Info {
			#ifdef ARCH_IS_64_BIT
			unsigned long id : 62;
			#else
			unsigned long id : 30;
			#endif
			
			unsigned gc_lock : 1;
			unsigned frozen : 1;
		} m_Info;
		
		explicit ThinObject(const Ptr<Object>& prototype) : m_Prototype(prototype) { init(); }
		ThinObject(const ThinObject& other) : m_Prototype(other.m_Prototype) { init(); }
		
		//virtual void gc_func(GCFunc func) { func(m_Prototype); }
		GC_ROOTS { GC_ROOT(m_Prototype); }
		bool gc_try_lock() { return m_Info.gc_lock ? false : (m_Info.gc_lock = true); }
		void gc_unlock() { m_Info.gc_lock = false; }
		void init();
	public:
		virtual ~ThinObject() {}
		
		virtual inline Value call(const Value& self, const Arguments&) { return self; }
		
		uintx id() const { return m_Info.id; }
		bool is_frozen() const { return m_Info.frozen; }
		virtual void freeze() { m_Info.frozen = true; }
		virtual void unfreeze() { m_Info.frozen = false; }
		
		virtual bool has_member(Symbol) const { return false; }
		virtual Value get_raw(Symbol name) const;
		virtual Value set_raw(Symbol name, const Value&);
		virtual Value get(const Value& self, Symbol member) const;
		virtual Value set(const Value& self, Symbol member, const Value& val);
		
		Ptr<Object> prototype() const { return m_Prototype ? m_Prototype : object_prototype(); }
		void set_prototype(const Ptr<Object>& proto) { m_Prototype = proto; }
	};
}

#endif /* end of include guard: THINOBJECT_H_JLSFLZC3 */
