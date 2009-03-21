#ifndef GARBAGEALLOCATOR_H_3WE55TYZ
#define GARBAGEALLOCATOR_H_3WE55TYZ

#include "MemoryManager.h"
#include <vector>

namespace snow {
	class GarbageAllocator : public IAllocator {
	public:
		typedef void(*FreeFunc)(void* ptr, size_t size);
	private:
		struct Heap {
			byte* m_Data;
			const size_t m_Size;
			size_t m_Offset;
			
			Heap(size_t sz);
			~Heap();
			size_t available() const { return m_Size - m_Offset; }
			bool contains(void* ptr) const { return m_Data <= (byte*)ptr && &m_Data[m_Offset] > (byte*)ptr; }
		};
		
		struct Header {
			// sizeof(Header) should be == 16 (128 bits)
			unsigned size           : 32;
			unsigned flags          : 16;
			uint32_t generation     : 16;
			FreeFunc free_func;  // : 64;
		};
		
		enum Flags {
			NO_FLAGS   = 0,
			MARKED     = 1,        // Object is referenced, don't delete
			BLOB       = 1 << 1    // Object doesn't have a destructor
		};
		
		IAllocator::Statistics m_Statistics;
		Heap* m_Nursery;
		Heap* m_Mature;
		std::vector<Heap*> m_RealLife;
		
		Heap& nursery();
		Heap& mature();
		void* allocate_from_heap(Heap&, size_t);
	public:
		GarbageAllocator();
		void* allocate(size_t sz);
		size_t size_of(void* ptr);
		const IAllocator::Statistics& statistics() const { return m_Statistics; }
		bool contains(void* ptr) const;
	};
}

#endif /* end of include guard: GARBAGEALLOCATOR_H_3WE55TYZ */
