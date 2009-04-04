#include "Garbage.h"
#include "MemoryManager.h"
#include "GarbageAllocator.h"

namespace snow {
	bool Garbage::is_blob(void* ptr) {
		return false;
	}
	
	size_t Garbage::generation(void* ptr) {
		return 0;
	}
	
	/*void Garbage::set_free_func(void* ptr, GarbageAllocator::FreeFunc func) {
		
	}
	
	GarbageAllocator::FreeFunc Garbage::free_func(void* ptr) {
		return NULL;
	}*/
	
	void Garbage::collect() {
		dynamic_cast<GarbageAllocator&>(MemoryManager::allocator(kGarbage)).collect();
	}

	void Garbage::register_root(IGarbage* ptr) {
		reinterpret_cast<GarbageAllocator&>(MemoryManager::allocator(kGarbage)).register_root(ptr);
	}
	
	void Garbage::unregister_root(IGarbage* ptr) {
		reinterpret_cast<GarbageAllocator&>(MemoryManager::allocator(kGarbage)).unregister_root(ptr);
	}
}
