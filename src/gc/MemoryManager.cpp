#include "MemoryManager.h"
#include <cstdlib>
#ifdef __APPLE__
#include <malloc/malloc.h>
#else
#include <malloc.h>
#endif

#include "GarbageAllocator.h"
#include "ExecutableAllocator.h"

#include <set>

#define DEBUG_FREES 0

namespace snow {
	#if DEBUG_FREES
	struct FreePointerListNode {
		FreePointerListNode* next;
		void* ptr;
	};
	
	static FreePointerListNode* __head = NULL;
	
	static void __insert(void* ptr) {
		FreePointerListNode* old_head = __head;
		__head = (FreePointerListNode*)malloc(sizeof(FreePointerListNode));
		__head->ptr = ptr;
		__head->next = old_head;
	}
	
	static bool __exists(void* ptr) {
		FreePointerListNode* node = __head;
		while (node) {
			if (node->ptr == ptr)
				return true;
			node = node->next;
		}
		return false;
	}
	
	static bool __erase(void* ptr) {
		FreePointerListNode* node = __head;
		FreePointerListNode* node_before = NULL;
		while (node) {
			if (node->ptr == ptr) {
				if (node_before) {
					node_before->next = node->next;
				} else {
					__head = node->next;
				}
				free(node);
				return true;
			}
			
			node_before = node;
			node = node->next;
		}
		return false;
	}
	#endif // DEBUG_FREES
	
	/*
		Simples possible IAllocator implementation.
	*/
	class MallocAllocator : public IAllocator {
	public:
		IAllocator::Statistics stats;
		
		void* allocate(size_t sz, AllocationType) {
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
#ifdef __APPLE__
			return malloc_size(ptr);
#else
			return malloc_usable_size(ptr);
#endif
		}
		
		const IAllocator::Statistics& statistics() const { return stats; }
	};
	
	IAllocator& MemoryManager::allocator(AllocatorType type) {
		switch (type) {
			default:
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

	IGarbageCollector& MemoryManager::collector() {
		// Could be cleaner...
		return dynamic_cast<GarbageAllocator&>(allocator(kGarbage));
	}
	
	void* MemoryManager::allocate(size_t sz, AllocatorType type, AllocationType allocation_type) {
		void* ptr = allocator(type).allocate(sz, allocation_type);
		
		#ifdef DEBUG
		// POISON
		memset(ptr, 0xcd, sz);
		#endif
		
		#if DEBUG_FREES
		while (__erase(ptr)) {}
		#endif
		
		return ptr;
	}
	
	void MemoryManager::free(void* ptr) {
		#if DEBUG_FREES
		if (__exists(ptr)) {
			error("double free()");
			TRAP();
		}
		__insert(ptr);
		#endif // DEBUG_FREES
		
		#ifdef DEBUG
		// POISON
		memset(ptr, 0xef, allocator(kMalloc).size_of(ptr));
		#endif 
		
		reinterpret_cast<MallocAllocator&>(allocator(kMalloc)).free(ptr);
	}
	
	const IAllocator::Statistics& MemoryManager::statistics(AllocatorType type) {
		return allocator(type).statistics();
	}
}
