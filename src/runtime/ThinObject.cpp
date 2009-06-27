#include "ThinObject.h"
#include "Object.h"
#include "SnowString.h"
#include "Exception.h"

namespace snow {
	static volatile uintx global_object_id_counter = 1;

	void ThinObject::init() {
		m_Info.frozen = false;
		m_Info.gc_lock = false;
		// XXX: With multithreading, this will most certainly go wrong.
		#ifdef ARCH_IS_64_BIT
		ASSERT(global_object_id_counter < (1LU<<61));
		#else
		ASSERT(global_object_id_counter < (1<<29));
		#endif
		m_Info.id = global_object_id_counter++;
	}

	Value ThinObject::get_raw(Symbol name) const {
		 return prototype()->get_raw(name);
	}
	
	Value ThinObject::set_raw(Symbol name, const Value&) {
		throw_exception(new String("Thin objects cannot have members assigned. Modify the prototype, or create a wrapper."));
		return nil();
	}
	
	Value ThinObject::set(const Value& self, Symbol member, const Value& val) {
		return prototype()->set(self, member, val);
	}

	Value ThinObject::get(const Value& self, Symbol member) const {
		return prototype()->get(self, member);
	}
}
