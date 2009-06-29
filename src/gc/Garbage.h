#ifndef GARBAGE_H_HYY44ECE
#define GARBAGE_H_HYY44ECE

#include "base/Basic.h"
#include "base/ThreadLocal.h"
#include "gc/GarbageAllocator.h"

namespace snow {
	class IGarbage;

	/*
		A set of garbage collection related utility functions.
		This is tightly coupled with GarbageAllocator.
	*/
	class Garbage {
	private:
		Garbage() {}
		static volatile bool s_Fence;
		static ThreadLocal<bool> s_AtFence;
	public:
		static bool is_marked(void* ptr);
		static bool is_blob(void* ptr);
		static size_t generation(void* ptr);
		//static void set_free_func(void* ptr, GarbageAllocator::FreeFunc func);
		//static GarbageAllocator::FreeFunc free_func(void* ptr);
		static void register_root(IGarbage* ptr);
		static void unregister_root(IGarbage* ptr);
		
		static void collect();

		static void init_fence();
		static void fence();
		static void lock_fence();
		static void unlock_fence();
	};

	inline void Garbage::init_fence() {
		//omp_init_lock(&s_Fence);
	}

	inline void Garbage::fence() {
		#pragma omp flush
		s_AtFence = true;
		while (s_Fence);
		s_AtFence = false;
	}


	inline void Garbage::unlock_fence() {
		s_Fence = false;
	}
	
	
	template <class T, typename... Args>
	inline Ptr<T> gc_new(Args... a) {
		static_assert(std::is_base_of<IGarbage, T>::value, "Can only use gc_new with classes that implement IGarbage.");
		HandleScope _;
		Handle<T> object = (T*)MemoryManager::allocate(sizeof(T), kGarbage, kObject);
		
		ASSERT(!IGarbage::is_in_constructor && "It is unsafe to perform GC allocations inside a GC constructor. Please use initialize() instead");
		IGarbage::is_in_constructor = true;
		::new(object.value()) T(a...); // call constructor using placement new
		IGarbage::is_in_constructor = false;
		
		object->initialize(a...);
		return object;
	}

	template <typename T>
	inline DataPtr<T> gc_new_array(size_t len) { 
		static_assert(std::is_pod<T>::value, "Garbage collector does not support array allocations of non-POD data types.");
		ASSERT(!IGarbage::is_in_constructor && "It is unsafe to perform GC allocations inside a GC constructor. Please use initialize() instead");
		return new(kGarbage, kBlob) T[len];
	}

	template <typename T, typename... Args>
	inline Ptr<T> malloc_new(Args... a) {
		static_assert(std::is_base_of<IGarbage, T>::value, "Can only use malloc_new with classes that implement IGarbage.");
		T* ret = new(kMalloc) T(a...);
		Garbage::register_root(ret);
		ret->initialize(a...);
		return ret;
	}

	template <typename T>
	inline void malloc_delete(const Ptr<T> obj) {
		static_assert(std::is_base_of<IGarbage, T>::value, "Can only use malloc_delete with classes that implement IGarbage.");
		Garbage::unregister_root(obj);
		delete obj.value();
	}
}

#endif /* end of include guard: GARBAGE_H_HYY44ECE */
