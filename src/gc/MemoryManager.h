#ifndef MEMORYMANAGER_H_WILYP9Q9
#define MEMORYMANAGER_H_WILYP9Q9

#include <new>
#include "base/Basic.h"

namespace snow {
	class GarbageAllocator;
	
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
		virtual void free(void* ptr) {}
		virtual size_t size_of(void* ptr) = 0;
		virtual const Statistics& statistics() const = 0;
		virtual bool contains(void* ptr) const { return false; }
	};
	
	class MemoryManager {
	public:
		static void* allocate(size_t sz, AllocatorType, AllocationType);
		static void free(void* ptr, AllocatorType);
		
		static IAllocator& allocator(AllocatorType);
		static GarbageAllocator& collector();
		static const IAllocator::Statistics& statistics(AllocatorType);

		template <typename Allocator>
		static Allocator& allocator() {
			static Allocator a;
			return a;
		}
	};
}

inline void* operator new(size_t sz) {
	return snow::MemoryManager::allocate(sz, snow::kMalloc, snow::kObject);
}

inline void* operator new(size_t sz, snow::AllocatorType type, snow::AllocationType allocation_type = snow::kObject) {
	ASSERT((type != snow::kGarbage || allocation_type != snow::kObject) && "Garbage-collected object allocations must implement IGarbage. If your object does not contain pointers to other garbage-collected objects, you may use the kBlob allocation type instead.");
	return snow::MemoryManager::allocate(sz, type, allocation_type);
}

inline void* operator new[](size_t sz) {
	return snow::MemoryManager::allocate(sz, snow::kMalloc, snow::kArray);
}

inline void* operator new[](size_t sz, snow::AllocatorType type, snow::AllocationType allocation_type = snow::kArray) {
	ASSERT(((type != snow::kGarbage) || (type == snow::kGarbage && allocation_type == snow::kBlob)) && "Garbage-collected object allocations must implement IGarbage.");
	return snow::MemoryManager::allocate(sz, type, allocation_type);
}

inline void operator delete(void* ptr) {
	snow::MemoryManager::free(ptr, snow::kMalloc);
}

inline void operator delete[](void* ptr) {
	snow::MemoryManager::free(ptr, snow::kMalloc);
}



#endif /* end of include guard: MEMORYMANAGER_H_WILYP9Q9 */
