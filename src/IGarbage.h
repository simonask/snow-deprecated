#ifndef IGARBAGE_H_XIM4X2BE
#define IGARBAGE_H_XIM4X2BE

#include "MemoryManager.h"
#include "Delegate.h"
#include "Garbage.h"

namespace snow {

#define GC_ROOTS virtual void _gc_roots(snow::IGarbageCollector& _gc, snow::IGarbageCollector::GCOperation _op)
#define GC_ROOTS_IMPL(klass) void klass::_gc_roots(snow::IGarbageCollector& _gc, snow::IGarbageCollector::GCOperation _op)
#define GC_ROOT(member) _gc.root_callback(_op, reinterpret_cast<void*&>(member))
#define GC_SUPER(superklass) superklass::_gc_roots(_gc, _op)

	class IGarbage {
	public:
		GC_ROOTS = 0;

		virtual bool gc_try_lock() = 0;
		virtual void gc_unlock() = 0;

		void* operator new(size_t sz, AllocatorType type = kGarbage);
		void operator delete(void* ptr);
	};

	inline void* IGarbage::operator new(size_t sz, AllocatorType type) {
		void* ptr = MemoryManager::allocate(sz, type, kObject);
		if (type != kGarbage) {
			Garbage::register_root(reinterpret_cast<IGarbage*>(ptr));
		}
		return ptr;
	}

	inline void IGarbage::operator delete(void* ptr) {
		Garbage::unregister_root(reinterpret_cast<IGarbage*>(ptr));
	}
}

#endif /* end of include guard: IGARBAGE_H_XIM4X2BE */
