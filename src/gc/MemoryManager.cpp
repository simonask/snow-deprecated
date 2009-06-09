#include "MemoryManager.h"
#include <cstdlib>
#ifdef __APPLE__
#include <malloc/malloc.h>
#else
#include <malloc.h>
#endif

#include "GarbageAllocator.h"
#include "ExecutableAllocator.h"
#include "runtime/Handle.h"

#include <set>

#define DEBUG_FREES 0

namespace snow {
	volatile bool IGarbage::is_in_constructor = false;

	#if DEBUG_FREES
	struct FreePointerListNode {
		FreePointerListNode* next;
		void* ptr;
	};
	
	static FreePointerListNode* __head = NULL;
	
	static void __insert(void* ptr) {
		#pragma omp critical(free_list)
		{
			FreePointerListNode* old_head = __head;
			__head = (FreePointerListNode*)malloc(sizeof(FreePointerListNode));
			__head->ptr = ptr;
			__head->next = old_head;
		}
	}
	
	static bool __exists(void* ptr) {
		bool ret = false;
		#pragma omp critical(free_list)
		{
			FreePointerListNode* node = __head;
			while (node) {
				if (node->ptr == ptr)
				{
					ret = true;
					break;
				}
				node = node->next;
			}
		}
		return ret;
	}
	
	static bool __erase(void* ptr) {
		bool ret = false;
		if (!__head)
			return false;
		#pragma omp critical(free_list)
		{
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
					ret = true;
					break;
				}
				
				node_before = node;
				node = node->next;
			}
		}
		return ret;
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
				return allocator<MallocAllocator>();
			}
			case kGarbage: {
				return allocator<GarbageAllocator>();
			}
			case kExecutable: {
				return allocator<ExecutableAllocator>();
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
	
	void MemoryManager::free(void* ptr, AllocatorType type) {
		ASSERT(!allocator<GarbageAllocator>().contains(ptr) && "Attempt to free gc'd pointer.");

		#if DEBUG_FREES
		if (__exists(ptr)) {
			error("double free()");
			TRAP();
		}
		__insert(ptr);
		#endif // DEBUG_FREES
		
		#ifdef DEBUG
		// POISON
		memset(ptr, 0xef, allocator(type).size_of(ptr));
		#endif 
		
		allocator(type).free(ptr);
	}
	
	const IAllocator::Statistics& MemoryManager::statistics(AllocatorType type) {
		return allocator(type).statistics();
	}
}

