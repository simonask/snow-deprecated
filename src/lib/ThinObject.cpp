#include "ThinObject.h"
#include "Object.h"

namespace snow {
	VALUE ThinObject::get(const std::string& name) const {
		 return m_Prototype->get(name);
	}
	
	VALUE ThinObject::set(const std::string& name, VALUE) {
		// TODO: Exception
		error("Thin objects cannot have members assigned. Modify the prototype, or create a wrapper.");
		TRAP();
		return nil();
	}
	
	VALUE ThinObject::va_call(VALUE self, uint64_t num_args, va_list&) {
		if (num_args > 0)
			debug("You called non-function object with %llu arguments.", num_args);
		return self;
	}
	
	Object* ThinObject::prototype() const {
		 return m_Prototype ? m_Prototype : &*object_prototype();
	}
}