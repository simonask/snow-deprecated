#include "Garbage.h"

namespace snow {
	void Garbage::mark(void* ptr) {
		
	}
	
	bool Garbage::is_marked(void* ptr) {
		
	}
	
	bool Garbage::is_blob(void* ptr) {
		
	}
	
	size_t Garbage::generation(void* ptr) {
		
	}
	
	void Garbage::set_free_func(void* ptr, GarbageAllocator::FreeFunc func) {
		
	}
	
	GarbageAllocator::FreeFunc Garbage::free_func(void* ptr) {
		
	}
	
	void Garbage::collect() {
		
	}
}