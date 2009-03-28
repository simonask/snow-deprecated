#include "Garbage.h"
#include "MemoryManager.h"

namespace snow {
	void Garbage::mark(void* ptr) {
		dynamic_cast<GarbageAllocator&>(MemoryManager::allocator(kGarbage)).mark(ptr);
	}
	
	bool Garbage::is_marked(void* ptr) {
		return false;	
	}
	
	bool Garbage::is_blob(void* ptr) {
		return false;
	}
	
	size_t Garbage::generation(void* ptr) {
		return 0;
	}
	
	void Garbage::set_free_func(void* ptr, GarbageAllocator::FreeFunc func) {
		
	}
	
	GarbageAllocator::FreeFunc Garbage::free_func(void* ptr) {
		return NULL;
	}
	
	void Garbage::collect() {
		dynamic_cast<GarbageAllocator&>(MemoryManager::allocator(kGarbage)).collect();
	}
}
