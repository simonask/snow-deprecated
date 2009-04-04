#ifndef IGARBAGE_H_XIM4X2BE
#define IGARBAGE_H_XIM4X2BE

#include "MemoryManager.h"
#include "Delegate.h"
#include "Garbage.h"

namespace snow {

typedef Delegate<void, void*&, const char*, int> GCFunc;
#define GC_ROOTS virtual void _gc_roots(GCFunc& _gc_func)
#define GC_ROOTS_IMPL(klass) void klass::_gc_roots(GCFunc& _gc_func)
#define GC_ROOT(member) _gc_func(reinterpret_cast<void*&>(member), __FILE__, __LINE__)
#define GC_SUPER(superklass) superklass::_gc_roots(_gc_func)

	class IGarbage {
	public:
		GC_ROOTS = 0;
		
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


	template <class T>
	class ExternalRoot {
	private:
		T m_Object;
	public:
		template <typename... Args>
		ExternalRoot(Args... a) : m_Object(a...) {
			Garbage::register_root(&m_Object);
		}
		~ExternalRoot() {
			Garbage::unregister_root(&m_Object);
		}
		
		T& object() { return m_Object; }
		const T& object() const { return m_Object; }

		T* operator->() const { return &m_Object; }
		operator T&() { return object(); }
		operator const T&() { return object(); }
	};
}

#endif /* end of include guard: IGARBAGE_H_XIM4X2BE */
