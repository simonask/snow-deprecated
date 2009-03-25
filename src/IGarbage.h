#ifndef IGARBAGE_H_XIM4X2BE
#define IGARBAGE_H_XIM4X2BE

#include "MemoryManager.h"

namespace snow {
	class IGarbage {
	public:
		virtual void gc_mark() = 0;
		
		void* operator new(size_t sz, AllocatorType type = kGarbage) { return ::operator new(sz, type); };
	};
}

#endif /* end of include guard: IGARBAGE_H_XIM4X2BE */
