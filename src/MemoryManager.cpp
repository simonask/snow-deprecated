#include "MemoryManager.h"
#include <cstdlib>
#include <malloc/malloc.h>

#include "GarbageAllocator.h"
#include "ExecutableAllocator.h"

namespace snow {
	
	/*
		Simples possible IAllocator implementation.
	*/
	class MallocAllocator : public IAllocator {
	public:
		IAllocator::Statistics stats;
		
		void* allocate(size_t sz) {
			void* ptr = std::malloc(sz);
			stats.allocated_size += size_of(ptr);
			stats.allocated_objects++;
			//debug("MALLOC: 0x%llx (%d bytes)", ptr, sz);
			return ptr;
		}
		
		void free(void* ptr) {
			stats.freed_size += size_of(ptr);
			stats.freed_objects++;
			std::free(ptr);
		}
		
		size_t size_of(void* ptr) {
			return malloc_size(ptr);
		}
		
		const IAllocator::Statistics& statistics() const { return stats; }
	};
	
	IAllocator& MemoryManager::allocator(AllocatorType type) {
		switch (type) {
			case kMalloc: {
				static MallocAllocator malloc_allocator;
				return malloc_allocator;
			}
			case kGarbage: {
				static GarbageAllocator garbage_allocator;
				return garbage_allocator;
			}
			case kExecutable: {
				static ExecutableAllocator executable_allocator;
				return executable_allocator;
			}
		}
	}
	
	void* MemoryManager::allocate(size_t sz, AllocatorType type) {
		void* ptr = allocator(type).allocate(sz);
		
		#ifdef DEBUG
		// POISON
		memset(ptr, 0xcd, sz);
		#endif
		
		return ptr;
	}
	
	void MemoryManager::free(void* ptr) {
		#ifdef DEBUG
		// POISON
		memset(ptr, 0xef, allocator(kMalloc).size_of(ptr));
		
		if (allocator(kGarbage).contains(ptr) || allocator(kExecutable).contains(ptr)) {
			error("free() on garbage-collected pointer!");
			TRAP();
		}
		#endif
		
		reinterpret_cast<MallocAllocator&>(allocator(kMalloc)).free(ptr);
	}
	
	const IAllocator::Statistics& MemoryManager::statistics(AllocatorType type) {
		return allocator(type).statistics();
	}
}

