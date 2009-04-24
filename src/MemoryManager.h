#ifndef MEMORYMANAGER_H_WILYP9Q9
#define MEMORYMANAGER_H_WILYP9Q9

#include "Basic.h"

#include "IGarbageCollector.h"

namespace snow {
	enum AllocatorType {
		kMalloc,
		kGarbage,
		kExecutable,
	};
	
	enum AllocationType {
		kObject,
		kBlob,
		kArray
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
		
		virtual void* allocate(size_t sz, AllocationType) = 0;
		virtual size_t size_of(void* ptr) = 0;
		virtual const Statistics& statistics() const = 0;
		virtual bool contains(void* ptr) const { return false; }
	};
	
	class MemoryManager {
	public:
		static void* allocate(size_t sz, AllocatorType, AllocationType);
		static void free(void* ptr);
		
		static IAllocator& allocator(AllocatorType);
		static IGarbageCollector& collector();
		static const IAllocator::Statistics& statistics(AllocatorType);
	};
};

inline void* operator new(size_t sz) {
	return snow::MemoryManager::allocate(sz, snow::kMalloc, snow::kObject);
}

inline void* operator new(size_t sz, snow::AllocatorType type) {
	ASSERT((type != snow::kGarbage) && "Garbage-collected object allocations must implement IGarbage.");
	return snow::MemoryManager::allocate(sz, type, snow::kObject);
}

inline void* operator new[](size_t sz) {
	return snow::MemoryManager::allocate(sz, snow::kMalloc, snow::kArray);
}

inline void* operator new[](size_t sz, snow::AllocatorType type, snow::AllocationType allocation_type = snow::kArray) {
	ASSERT(((type != snow::kGarbage) || (type == snow::kGarbage && allocation_type == snow::kBlob)) && "Garbage-collected object allocations must implement IGarbage.");
	return snow::MemoryManager::allocate(sz, type, allocation_type);
}

inline void operator delete(void* ptr) {
	snow::MemoryManager::free(ptr);
}

inline void operator delete[](void* ptr) {
	snow::MemoryManager::free(ptr);
}

#endif /* end of include guard: MEMORYMANAGER_H_WILYP9Q9 */
