#ifndef ARRAY_H_QGTWXC5G
#define ARRAY_H_QGTWXC5G

#include "Handle.h"

namespace snot {
	template <class T>
	class Array {
	private:
		T* m_Ptr;
		size_t m_Length;
		void populate(const T* origin) {
			delete [] m_Ptr;
			m_Ptr = new T[m_Length];
			for (int i = 0; i < m_Length; ++i) {
				m_Ptr[i] = origin[i];
			}
		}
	public:
		class Ref {
		friend class Array<T>;
		private:
			T& m_Ref;
			Ref(T& ref) : m_Ref(ref) {}
		public:
			T& operator=(const T& other) { m_Ref = other; }
			operator T&() const { return m_Ref; }
			operator const T&() const { return m_Ref; }
			operator Handle<T>() const { return Handle<T>(&m_Ref); }
		};
		
		Array(const T&* ptr, size_t len) : m_Ptr(NULL),  m_Length(len) { populate(ptr); }
		Array(const Array<T>& other) : m_Ptr(NULL), m_Length(other.m_Length) { populate(ptr); }
		~Array() { delete [] m_Ptr; }
		Ref<T> operator[](size_t index) { return Ref(m_Ptr[index]); }
		size_t length() const { return m_Length; }
		size_t size() const { return m_Length; }
	};
}

#endif /* end of include guard: ARRAY_H_QGTWXC5G */
