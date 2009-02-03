#ifndef GARBAGE_H_N12SKHS0
#define GARBAGE_H_N12SKHS0

#include "Basic.h"

namespace snow {
	class Garbage {
		friend class MemoryHeap;
	private:
		size_t m_Size;
		int m_Mark;
	protected:
		void mark() { ++m_Mark; }
		void unmark() { m_Mark = 0; }
	public:
		virtual ~Garbage() {}
		
		void* operator new(size_t sz) throw();
		void operator delete(void*) {};
		
		static void collect();
	};
}

#endif /* end of include guard: GARBAGE_H_N12SKHS0 */
