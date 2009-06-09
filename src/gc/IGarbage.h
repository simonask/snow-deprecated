#ifndef IGARBAGE_H_XIM4X2BE
#define IGARBAGE_H_XIM4X2BE

#include "MemoryManager.h"
#include "Garbage.h"
#include "runtime/Handle.h"

namespace snow {

#define GC_ROOTS virtual void _gc_roots(snow::IGarbageCollector& _gc, snow::IGarbageCollector::GCOperation _op)
#define GC_ROOTS_IMPL(klass) void klass::_gc_roots(snow::IGarbageCollector& _gc, snow::IGarbageCollector::GCOperation _op)
#define GC_ROOT(member) _gc.root_callback(_op, reinterpret_cast<void*&>(member))
#define GC_NONPOINTER_ROOT(member) (member)._gc_roots(_gc, _op)
#define GC_SUPER(superklass) superklass::_gc_roots(_gc, _op)

#define NO_INIT template <typename... Args> void initialize(Args... a) {}

	class IGarbage {
	private:
		template<class T, typename... Args> friend T* gc_new(Args...);
		template<class T, typename... Args> friend T* malloc_new(Args...);
		template<class T> friend void malloc_delete(T*);
		template<typename T> friend T* gc_new_array(size_t);
		static volatile bool is_in_constructor;
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

	template <class T, typename... Args>
	T* gc_new(Args... a) {
		static_assert(std::is_base_of<IGarbage, T>::value, "Can only use gc_new with classes that implement IGarbage.");
		HandleScope _;
		ASSERT(!IGarbage::is_in_constructor && "It is unsafe to perform GC allocations inside a GC constructor. Please use initialize() instead");
		IGarbage::is_in_constructor = true;
		Handle<T> ret = new(kGarbage) T(a...);
		IGarbage::is_in_constructor = false;
		ret->initialize(a...);
		return ret;
	}

	template <typename T>
	T* gc_new_array(size_t len) { 
		static_assert(std::is_pod<T>::value, "Garbage collector does not support array allocations of non-POD data types.");
		ASSERT(!IGarbage::is_in_constructor && "It is unsafe to perform GC allocations inside a GC constructor. Please use initialize() instead");
		return new(kGarbage, kBlob) T[len];
	}

	template <typename T, typename... Args>
	T* malloc_new(Args... a) {
		static_assert(std::is_base_of<IGarbage, T>::value, "Can only use malloc_new with classes that implement IGarbage.");
		T* ret = new(kMalloc) T(a...);
		Garbage::register_root(ret);
		ret->initialize(a...);
		return ret;
	}

	template <typename T>
	void malloc_delete(T* obj) {
		static_assert(std::is_base_of<IGarbage, T>::value, "Can only use malloc_delete with classes that implement IGarbage.");
		Garbage::unregister_root(obj);
		delete obj;
	}
}

#endif /* end of include guard: IGARBAGE_H_XIM4X2BE */
