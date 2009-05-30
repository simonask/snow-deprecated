#include "Garbage.h"
#include "MemoryManager.h"
#include "GarbageAllocator.h"
#include "runtime/Runtime.h"

namespace snow {
	volatile bool Garbage::s_Fence = false;
	ThreadLocal<bool> Garbage::s_AtFence = false;

	bool Garbage::is_blob(void* ptr) {
		return false;
	}
	
	size_t Garbage::generation(void* ptr) {
		return 0;
	}
	
	/*void Garbage::set_free_func(void* ptr, GarbageAllocator::FreeFunc func) {
		
	}
	
	GarbageAllocator::FreeFunc Garbage::free_func(void* ptr) {
		return NULL;
	}*/
	
	void Garbage::collect() {
		MemoryManager::allocator<GarbageAllocator>().collect();
	}

	void Garbage::register_root(IGarbage* ptr) {
		MemoryManager::allocator<GarbageAllocator>().register_root(ptr);
	}
	
	void Garbage::unregister_root(IGarbage* ptr) {
		MemoryManager::allocator<GarbageAllocator>().unregister_root(ptr);
	}

	void Garbage::lock_fence() {
		debug("locking fence from thread %d", omp_get_thread_num());
		ASSERT(!s_Fence);
		s_Fence = true;
		debug("fence locked from thread %d", omp_get_thread_num());
		// wait for all active threads to reach the fence before continuing
		ThreadLocal<StackFrame*>& stack_frames(get_current_stack_frames());
		bool is_any_not_at_fence = true;
		while (is_any_not_at_fence) {
			is_any_not_at_fence = false;
			for (int i = 0; i < omp_get_num_threads(); ++i) {
				if (i == omp_get_thread_num())
					continue;
				if (stack_frames.object_for_thread(i) && !s_AtFence.object_for_thread(i)) {
					debug("thread %d is not ready for gc yet", i);
					is_any_not_at_fence = true;
					break;
				}
			}
		}
	}
}
