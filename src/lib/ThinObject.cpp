#include "ThinObject.h"
#include "Object.h"
#include "SnowString.h"
#include "Exception.h"

namespace snow {
	static volatile uint64_t global_object_id_counter = 0;

	void ThinObject::init() {
		m_Info.frozen = false;
		m_Info.gc_lock = false;
		// XXX: With multithreading, this will most certainly go wrong.
		ASSERT(global_object_id_counter < (1LU<<61));
		m_Info.id = global_object_id_counter++;
	}

	VALUE ThinObject::get(VALUE name) const {
		 return m_Prototype->get(name);
	}
	
	VALUE ThinObject::set(VALUE name, VALUE) {
		throw_exception(new String("Thin objects cannot have members assigned. Modify the prototype, or create a wrapper."));
		return nil();
	}
	
	VALUE ThinObject::va_call(VALUE self, uint64_t num_args, va_list&) {
		throw_exception(new String("Called a non-function object. This is probably a mistake."));
		return self;
	}
	
	Object* ThinObject::prototype() const {
		 return m_Prototype ? m_Prototype : &*object_prototype();
	}
}
