#ifndef EXECUTABLEALLOCATOR_H_E9237MC1
#define EXECUTABLEALLOCATOR_H_E9237MC1

#include "MemoryManager.h"
#include <vector>

namespace snow {
	class ExecutableAllocator : public IAllocator {
	private:
		class IncrementalHeap;
		
		std::vector<IncrementalHeap*> m_Heaps;
		IAllocator::Statistics m_Stats;
		
		IncrementalHeap* heap_containing(void* ptr);
	public:
		void* allocate(size_t sz, AllocationType);
		size_t size_of(void* ptr) { return 0;}
		const IAllocator::Statistics& statistics() const { return m_Stats; }
		
		void make_executable(void* ptr);
		void make_writable(void* ptr);
	};
}

#endif /* end of include guard: EXECUTABLEALLOCATOR_H_E9237MC1 */
