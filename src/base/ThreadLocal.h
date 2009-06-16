#ifndef THREADLOCAL_H_87APQNCS
#define THREADLOCAL_H_87APQNCS

#include "base/Basic.h"
#include "gc/MemoryManager.h"
#include <omp.h>

namespace snow {
	template <typename T>
	class ThreadLocal {
	private:
		T m_DefaultValue;
		T* m_Array;
		size_t m_NumThreads;
		omp_lock_t m_Lock;

		void resize(size_t num_real_threads) {
			static_assert(std::is_pod<T>::value, "Cannot use non-POD types in ThreadLocals (yet).");
			T* old_array = m_Array;
			m_Array = new(kMalloc, kBlob) T[num_real_threads];
			if (!old_array)
				m_NumThreads = 0;
			for (size_t i = 0; i < num_real_threads; ++i) {
				m_Array[i] = m_DefaultValue;
			}
			for (size_t i = 0; i < m_NumThreads; ++i) {
				m_Array[i] = old_array[i];
			}
			delete[] old_array;
			m_NumThreads = num_real_threads;
		}

		inline void resize_check() {
			size_t num_real_threads = omp_get_num_threads();
			if (m_NumThreads >= num_real_threads && m_Array)
				return;
			resize(num_real_threads);
		}
	public:
		ThreadLocal() : m_DefaultValue(), m_Array(NULL), m_NumThreads(0) {
			omp_init_lock(&m_Lock);
		}

		ThreadLocal(const T& default_value) : m_DefaultValue(default_value), m_Array(NULL), m_NumThreads(0) {
			omp_init_lock(&m_Lock);
			resize_check();
		}

		ThreadLocal(const ThreadLocal<T>& other) : m_NumThreads(other.m_NumThreads) {
			omp_init_lock(&m_Lock);
			m_Array = new T[m_NumThreads];
			for (size_t i = 0; i < m_NumThreads; ++i) {
				m_Array[i] = other.m_Array[i];
			}
		}

		~ThreadLocal() {
			delete [] m_Array;
			omp_destroy_lock(&m_Lock);
		}

		/*
			ThreadLocal does not return references, because m_Array might be
			reallocated between subsequent calls.
		*/

		operator T() { return object(); }

		T object() {
			return object_for_thread(omp_get_thread_num());
		}

		ThreadLocal<T>& operator=(const T& other) {
			set_object_for_thread(omp_get_thread_num(), other);
			return *this;
		}

		size_t num_threads() const { return m_NumThreads; }

		T object_for_thread(size_t num) {
			omp_set_lock(&m_Lock);
			resize_check();
			ASSERT(num < m_NumThreads);
			T ret = m_Array[num];
			omp_unset_lock(&m_Lock);
			return ret;
		}

		T object_for_thread(size_t num) const {
			ASSERT(num < m_NumThreads);
			T ret = m_Array[num];
			return ret;
		}

		void set_object_for_thread(size_t num, const T& obj) {
			omp_set_lock(&m_Lock);
			resize_check();
			ASSERT(num < m_NumThreads);
			m_Array[num] = obj;
			omp_unset_lock(&m_Lock);
		}
	};
}

#endif // THREADLOCAL_H_87APQNCS

