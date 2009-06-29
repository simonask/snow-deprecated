#ifndef IGARBAGE_H_XIM4X2BE
#define IGARBAGE_H_XIM4X2BE

#include "MemoryManager.h"
#include "runtime/Handle.h"
#include "gc/GarbageTypes.h"

namespace snow {

#define GC_ROOTS virtual void _gc_roots(snow::GarbageAllocator& _gc, snow::GCOperation _op)
#define GC_ROOTS_IMPL(klass) void klass::_gc_roots(snow::GarbageAllocator& _gc, snow::GCOperation _op)
#define GC_ROOT(member) _gc.root_callback(_op, member)
#define GC_NONPOINTER_ROOT(member) (member)._gc_roots(_gc, _op)
#define GC_SUPER(superklass) superklass::_gc_roots(_gc, _op)

#define NO_INIT template <typename... Args> void initialize(Args... a) {}

	class IGarbage {
	private:
		template<class T, typename... Args> friend Ptr<T> gc_new(Args...);
		template<class T, typename... Args> friend Ptr<T> malloc_new(Args...);
		template<class T> friend void malloc_delete(const Ptr<T>&);
		template<typename T> friend DataPtr<T> gc_new_array(size_t);
		static ThreadLocal<bool> is_in_constructor;
	public:
		GC_ROOTS = 0;

		virtual bool gc_try_lock() = 0;
		virtual void gc_unlock() = 0;

	protected:
		void* operator new(size_t sz, AllocatorType type = kGarbage) {
			return MemoryManager::allocate(sz, type, kObject);
		}
		void operator delete(void* ptr) {
			return MemoryManager::free(ptr, kMalloc);
		}
	};
}

#endif /* end of include guard: IGARBAGE_H_XIM4X2BE */
