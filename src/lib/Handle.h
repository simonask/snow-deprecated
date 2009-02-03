#ifndef HANDLE_H_3UB7BMPV
#define HANDLE_H_3UB7BMPV

#include "Value.h"
#include "Basic.h"

namespace snow {
	class Handle {
	private:
		VALUE m_Value;
		Handle* m_Previous;
		Handle* m_Next;
	public:
		Handle(VALUE val = NULL);
		~Handle();
		
		VALUE value() const { return m_Value; }
		operator VALUE() const { return m_Value; }
		operator Object*() const;
		Handle& operator=(VALUE val) { m_Value = val; return *this; }
		operator bool() const { return eval_truth(m_Value); }
		bool operator==(const VALUE val) const { return m_Value == val; }
		Object* operator->();
		const Object* operator->() const;
		
		Handle* previous() const { return m_Previous; }
		Handle* next() const { return m_Next; }
		static Handle* last();
		
		bool is_object() const { return snow::is_object(m_Value); }
		template <typename T>
		T* cast() const { return snow::object_cast<T>(m_Value); }
	};
	
	inline VALUE value(const Handle& h) { return h.value(); }
}

#endif /* end of include guard: HANDLE_H_3UB7BMPV */
