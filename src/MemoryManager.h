#ifndef MEMORYMANAGER_H_WILYP9Q9
#define MEMORYMANAGER_H_WILYP9Q9

#include "Basic.h"

namespace snow {
	enum AllocatorType {
		kMalloc,
		kGarbage,
		kExecutable,
	};
	
	class IAllocator {
	protected:
		IAllocator() {}
	public:
		struct Statistics {
			size_t allocated_size;
			size_t allocated_objects;
			size_t freed_size;
			size_t freed_objects;

			// I'm lazy...
			Statistics() { memset(this, 0, sizeof(*this)); }
		};
		
		virtual void* allocate(size_t sz) = 0;
		virtual size_t size_of(void* ptr) = 0;
		virtual const Statistics& statistics() const = 0;
		virtual bool contains(void* ptr) const { return false; }
	};
	
	class MemoryManager {
	public:
		static void* allocate(size_t sz, AllocatorType);
		static void free(void* ptr);
		
		static IAllocator& allocator(AllocatorType);
		static const IAllocator::Statistics& statistics(AllocatorType);
	};
};

inline void* operator new(size_t sz) {
	return snow::MemoryManager::allocate(sz, snow::kMalloc);
}

inline void* operator new(size_t sz, snow::AllocatorType type) {
	return snow::MemoryManager::allocate(sz, type);
}

inline void* operator new[](size_t sz) {
	return snow::MemoryManager::allocate(sz, snow::kMalloc);
}

inline void* operator new[](size_t sz, snow::AllocatorType type) {
	return snow::MemoryManager::allocate(sz, type);
}

inline void operator delete(void* ptr) {
	snow::MemoryManager::free(ptr);
}

inline void operator delete[](void* ptr) {
	snow::MemoryManager::free(ptr);
}

#endif /* end of include guard: MEMORYMANAGER_H_WILYP9Q9 */
