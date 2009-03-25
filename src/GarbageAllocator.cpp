#include "GarbageAllocator.h"
#include "IGarbage.h"
#include "Handle.h"
#include "lib/Runtime.h"

#include <set>

namespace snow {
	static const size_t NURSERY_SIZE = (1<<20);     // 1 Mb
	static const size_t MATURE_SIZE = (1<<25);      // 32 Mb
	static const size_t REALLIFE_SIZE = (1<<23);    // 8 Mb per RealLife heap

	static const size_t ALIGNMENT = 0x10;
	
	static std::set<void*> POINTERS = std::set<void*>();
	
	enum Flags {
		NO_FLAGS        = 0,
		FLAG_MARKED     = 1,        // Object is referenced, don't delete
		FLAG_BLOB       = 1 << 1,   // Object doesn't have a destructor
	};
	
	struct Header {
		// sizeof(Header) should be == 16 (128 bits)
		unsigned size           : 32;
		unsigned flags          : 16;
		unsigned generation     : 16;
		GarbageAllocator::FreeFunc free_func;  // : 64;
	};
	
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
	
	static void* allocate_from_heap(GarbageAllocator::Heap& heap, size_t sz, Header** pheader) {
		size_t required_size = sizeof(Header) + sz;
		required_size += (ALIGNMENT - required_size % ALIGNMENT);
		
		if (heap.available() < required_size)
			return NULL;
			
		byte* data = &heap.m_Data[heap.m_Offset];
		*pheader = reinterpret_cast<Header*>(&data[0]);
		void* object = reinterpret_cast<void*>(&data[sizeof(Header)]);

		heap.m_Offset += required_size;
		
		
		(*pheader)->size = required_size - sizeof(Header);
		
		return object;
	}
	
	void* GarbageAllocator::allocate(size_t sz, snow::AllocationType type) {
		Header* header;
		void* ptr = allocate_from_heap(nursery(), sz, &header);
		
		header->flags = NO_FLAGS;
		header->generation = 0;
		header->free_func = NULL;
		
		switch (type) {
			case kArray:
				error("Garbage collector does not support array allocations. Use kBlob allocation if you need a block consisting of a POD type.");
				TRAP();
			case kBlob:
				header->flags |= FLAG_BLOB;
				break;
			case kObject:
				break;
		}
		
		if (!ptr) {
			// TODO: Minor collection
		}
		m_Statistics.allocated_size += sz;
		m_Statistics.allocated_objects++;
		POINTERS.insert(ptr);
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
	
	static void unmark_heap(GarbageAllocator::Heap& heap) {
		byte* data = heap.m_Data;
		while (data < &heap.m_Data[heap.m_Offset]) {
			Header* header = reinterpret_cast<Header*>(data);
			
			header->flags &= ~FLAG_MARKED;
			
			data += sizeof(*header) + header->size;
		}
	}
	
	static void call_destructors(GarbageAllocator::Heap& heap) {
		byte* data = heap.m_Data;
		while (data < &heap.m_Data[heap.m_Offset]) {
			Header* header = reinterpret_cast<Header*>(data);
			IGarbage* object = reinterpret_cast<IGarbage*>(&data[sizeof(*header)]);
			
			if (!(header->flags & FLAG_MARKED) && !(header->flags & FLAG_BLOB)) {
				object->~IGarbage();
			}
			
			data += sizeof(*header) + header->size;
		}
	}
	
	void GarbageAllocator::mark(void* ptr) {
		if (contains(ptr)) {
			byte* object = static_cast<byte*>(ptr);
			Header* header = reinterpret_cast<Header*>(object - (ptrdiff_t)sizeof(Header));
			header->flags |= FLAG_MARKED;
			if (!(header->flags & FLAG_BLOB)) {
				IGarbage* gc_object = reinterpret_cast<IGarbage*>(object);
				gc_object->gc_mark();
			}
		}
	}
	
	void GarbageAllocator::collect() {
		unmark_heap(nursery());
		
		for each (iter, ValueHandle::list()) {
			ValueHandle* handle = *iter;
			mark(handle->value());
		}
		
		StackFrame* frame = get_current_stack_frame();
		while (frame) {
			mark(frame->scope);
			frame = frame->previous;
		}
		
		call_destructors(nursery());
	}
}