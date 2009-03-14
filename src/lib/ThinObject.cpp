#include "ThinObject.h"
#include "Object.h"

namespace snow {
	VALUE ThinObject::call(const ValueHandle& self, uint64_t num_args, ...) {
		va_list ap;
		va_start(ap, num_args);
		VALUE ret = va_call(self, num_args, ap);
		va_end(ap);
		return ret;
	}
	
	VALUE ThinObject::get(const std::string& name) const {
		 return m_Prototype->get(name);
	}
	
	VALUE ThinObject::set(const std::string& name, VALUE) {
		// TODO: Exception
		error("Thin objects cannot have members assigned. Modify the prototype, or create a wrapper.");
		TRAP();
		return nil();
	}
	
	Handle<Object> ThinObject::prototype() const {
		 return m_Prototype ? Handle<Object>(m_Prototype) : object_prototype();
	}
}