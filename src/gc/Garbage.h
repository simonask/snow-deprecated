#ifndef GARBAGE_H_HYY44ECE
#define GARBAGE_H_HYY44ECE

#include "base/Basic.h"

namespace snow {
	class IGarbage;

	/*
		A set of garbage collection related utility functions.
		This is tightly coupled with GarbageAllocator.
	*/
	class Garbage {
	private:
		Garbage() {}
	public:
		static bool is_marked(void* ptr);
		static bool is_blob(void* ptr);
		static size_t generation(void* ptr);
		//static void set_free_func(void* ptr, GarbageAllocator::FreeFunc func);
		//static GarbageAllocator::FreeFunc free_func(void* ptr);
		static void register_root(IGarbage* ptr);
		static void unregister_root(IGarbage* ptr);
		
		static void collect();
	};
}

#endif /* end of include guard: GARBAGE_H_HYY44ECE */