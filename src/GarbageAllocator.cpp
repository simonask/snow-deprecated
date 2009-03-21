#include "GarbageAllocator.h"

namespace snow {
	static const size_t NURSERY_SIZE = (1<<20);     // 1 Mb
	static const size_t MATURE_SIZE = (1<<25);      // 32 Mb
	static const size_t REALLIFE_SIZE = (1<<23);    // 8 Mb per RealLife heap

	static const size_t ALIGNMENT = 0x10;
	
	GarbageAllocator::Heap::Heap(size_t size) : m_Data(NULL), m_Size(size), m_Offset(0) {
		m_Data = new(kMalloc) byte[size];
	}
	
	GarbageAllocator::Heap::~Heap() {
		delete[] m_Data;
	}
	
	GarbageAllocator::GarbageAllocator() : m_Nursery(NULL), m_Mature(NULL) {}
	
	GarbageAllocator::Heap& GarbageAllocator::nursery() {
		if (!m_Nursery)
			m_Nursery = new(kMalloc) Heap(NURSERY_SIZE);
		ASSERT(m_Nursery);
		return *m_Nursery;
	}
	
	GarbageAllocator::Heap& GarbageAllocator::mature() {
		if (!m_Mature)
			m_Mature = new(kMalloc) Heap(MATURE_SIZE);
		ASSERT(m_Mature);
		return *m_Mature;
	}
	
	void* GarbageAllocator::allocate_from_heap(GarbageAllocator::Heap& heap, size_t sz) {
		size_t required_size = sizeof(Header) + sz;
		required_size += (ALIGNMENT - required_size % ALIGNMENT);
		
		if (heap.available() < required_size)
			return NULL;
			
		byte* data = &heap.m_Data[heap.m_Offset];
		Header* header = reinterpret_cast<Header*>(&data[0]);
		void* object = reinterpret_cast<void*>(&data[sizeof(Header)]);

		heap.m_Offset += required_size;
		
		header->size = required_size - sizeof(Header);
		header->flags = NO_FLAGS;
		header->generation = 0;
		header->free_func = NULL;
		
		return object;
	}
	
	void* GarbageAllocator::allocate(size_t sz) {
		void* ptr = allocate_from_heap(nursery(), sz);
		if (!ptr) {
			// TODO: Minor collection
		}
		return ptr;
	}
	
	size_t GarbageAllocator::size_of(void* ptr) {
		return 0;
	}
	
	bool GarbageAllocator::contains(void* ptr) const {
		if (m_Nursery && m_Nursery->contains(ptr)) {
			return true;
		}
		if (m_Mature && m_Mature->contains(ptr)) {
			return true;
		}
		
		for each (iter, m_RealLife) {
			if ((*iter)->contains(ptr))
				return true;
		}
		
		return false;
	}
}