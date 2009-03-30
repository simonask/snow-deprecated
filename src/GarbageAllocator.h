#ifndef GARBAGEALLOCATOR_H_3WE55TYZ
#define GARBAGEALLOCATOR_H_3WE55TYZ

#include "MemoryManager.h"
#include <vector>

namespace snow {
	class GarbageAllocator : public IAllocator {
	public:
		typedef void(*FreeFunc)(void* ptr, size_t size);

		struct Heap {
			byte* m_Data;
			const size_t m_Size;
			size_t m_Offset;
			
			Heap(size_t sz);
			~Heap();
			size_t available() const { return m_Size - m_Offset; }
			bool contains(void* ptr) const { return m_Data <= (byte*)ptr && &m_Data[m_Offset] > (byte*)ptr; }
		};
		
	private:
		typedef void(*WalkFunc)(void** ptr, void* userdata);

		IAllocator::Statistics m_Statistics;
		Heap* m_Nursery;
		Heap* m_Mature;
		std::vector<Heap*> m_RealLife;
		
		Heap& nursery();
		Heap& mature();

		int64_t for_each_root(WalkFunc func, void* userdata = NULL);
	public:
		GarbageAllocator();
		void* allocate(size_t sz, AllocationType type);
		size_t size_of(void* ptr);
		const IAllocator::Statistics& statistics() const { return m_Statistics; }
		bool contains(void* ptr) const;
		
		void collect();
		void mark(void* ptr);
	};
}

#endif /* end of include guard: GARBAGEALLOCATOR_H_3WE55TYZ */
