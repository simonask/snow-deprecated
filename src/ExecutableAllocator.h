#ifndef EXECUTABLEALLOCATOR_H_E9237MC1
#define EXECUTABLEALLOCATOR_H_E9237MC1

#include "MemoryManager.h"

namespace snow {
	class ExecutableAllocator : public IAllocator {
	private:
		IAllocator::Statistics m_Stats;
	public:
		void* allocate(size_t sz);
		size_t size_of(void* ptr) { return 0;}
		const IAllocator::Statistics& statistics() const { return m_Stats; }
	};
}

#endif /* end of include guard: EXECUTABLEALLOCATOR_H_E9237MC1 */
