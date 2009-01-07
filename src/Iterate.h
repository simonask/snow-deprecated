#ifndef ITERATE_H_R6O1HEXF
#define ITERATE_H_R6O1HEXF

namespace snot {
	template <typename T, typename I = typename T::iterator, typename V = typename T::value_type>
	class Iterate {
	private:
		T& m_Container;
		I m_Iterator;
		I m_Begin;
		I m_End;
	public:
		Iterate(T& container, I begin, I end) : m_Container(container), m_Iterator(begin), m_Begin(begin), m_End(end) {}
		Iterate<T, I, V>& operator++() { m_Iterator++; return *this; }
		Iterate<T, I, V> operator++(int) { ++m_Iterator; return *this; }
		operator bool() const { return m_Iterator != m_End; }
		I operator->() { return m_Iterator; }
		V operator*() { return *m_Iterator; }
	};
	
	template <typename T>
	Iterate<T> iterate(T& container) {
		return Iterate<T>(container, container.begin(), container.end());
	}
	
	template <typename T>
	Iterate<const T, typename T::const_iterator> iterate(const T& container) {
		return Iterate<const T, typename T::const_iterator>(container, container.begin(), container.end());
	}
	
	template <typename T>
	Iterate<T, typename T::reverse_iterator> riterate(T& container) {
		return Iterate<T, typename T::reverse_iterator>(container, container.rbegin(), container.rend());
	}
	
	template <typename T>
	Iterate<const T, typename T::reverse_iterator> riterate(const T& container) {
		return Iterate<const T, typename T::reverse_iterator>(container, container.rbegin(), container.rend());
	}
}

#define each(ITER, COLLECTION) (auto ITER = snot::iterate(COLLECTION); ITER; ++ITER)

#endif /* end of include guard: ITERATE_H_R6O1HEXF */
