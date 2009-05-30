#ifndef GARBAGE_H_HYY44ECE
#define GARBAGE_H_HYY44ECE

#include "base/Basic.h"
#include "base/ThreadLocal.h"

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
}

#endif /* end of include guard: GARBAGE_H_HYY44ECE */
