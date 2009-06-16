#include "ThinObject.h"
#include "Object.h"
#include "SnowString.h"
#include "Exception.h"

namespace snow {
	static volatile uint64_t global_object_id_counter = 1;

	void ThinObject::init() {
		m_Info.frozen = false;
		m_Info.gc_lock = false;
		// XXX: With multithreading, this will most certainly go wrong.
		ASSERT(global_object_id_counter < (1LU<<61));
		m_Info.id = global_object_id_counter++;
	}

	VALUE ThinObject::get_raw(VALUE name) const {
		 return prototype()->get_raw(name);
	}
	
	VALUE ThinObject::set_raw(VALUE name, VALUE) {
		throw_exception(new String("Thin objects cannot have members assigned. Modify the prototype, or create a wrapper."));
		return nil();
	}
	
	VALUE ThinObject::set(VALUE self, VALUE member, VALUE val) {
		return prototype()->set(self, member, val);
	}

	VALUE ThinObject::get(VALUE self, VALUE member) const {
		return prototype()->get(self, member);
	}
}
