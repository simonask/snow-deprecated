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
#define GC_LOCK(mutex) auto __gc_lock = make_autolock(mutex); if(!__gc_lock.locked_by_me()) {return;}

	class IGarbage {
	public:
		bool __debug_gc;
		IGarbage() : __debug_gc(false) {}
		void __gc_debug(bool b) { __debug_gc = b; }

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

	template <typename MutexType>
	class AutoLock {
	private:
		MutexType& m_Mutex;
		bool m_LockedByMe;
	public:
		AutoLock(MutexType& mutex) : m_Mutex(mutex), m_LockedByMe(false) { m_LockedByMe = m_Mutex.try_lock(); }
		~AutoLock() { if (m_LockedByMe) m_Mutex.unlock(); }

		MutexType& mutex() { return m_Mutex; }
		const MutexType& mutex() const { return m_Mutex; }

		bool locked_by_me() const { return m_LockedByMe; }
	};

	template <typename MutexType>
	AutoLock<MutexType> make_autolock(MutexType& mutex) {
		return AutoLock<MutexType>(mutex);
	}
}

#endif /* end of include guard: IGARBAGE_H_XIM4X2BE */
