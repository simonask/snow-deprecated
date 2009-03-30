#ifndef IGARBAGE_H_XIM4X2BE
#define IGARBAGE_H_XIM4X2BE

#include "MemoryManager.h"
#include "Delegate.h"

namespace snow {
	class IGarbage {
	public:
		typedef Delegate<void, VALUE> GCFunc;

		virtual void gc_func(GCFunc func) = 0;
		
		void* operator new(size_t sz, AllocatorType type = kGarbage) { return ::operator new(sz, type); };
	};
}

#endif /* end of include guard: IGARBAGE_H_XIM4X2BE */
