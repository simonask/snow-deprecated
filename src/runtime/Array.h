#ifndef ARRAY_H_SCRHOXUE
#define ARRAY_H_SCRHOXUE

#include "Object.h"
#include <vector>

namespace snow {
	Object* array_prototype();
	
	class Array : public Object {
	private:
		GC_ROOTS;

		VALUE* m_Data;
		size_t m_Length;
		size_t m_AllocatedLength;
		
		void resize(size_t new_length);

		Array(const Array&) = delete;
	public:
		Array();
		
		IObject* copy() const { return Array::copy(*this); }

		virtual VALUE va_call(VALUE self, uint64_t num_args, va_list&);
		
		void set_data(VALUE* data, size_t len);
		void set_reference(VALUE* data, size_t len);
		void preallocate(size_t length);
		size_t length() const { return m_Length; }
		size_t allocated_length() const { return m_AllocatedLength; }
		VALUE* data() const { return m_Data; }
		
		VALUE get_by_index(int64_t index) const { return (*this)[index]; }
		VALUE set_by_index(int64_t index, VALUE val) { return (*this)[index] = val; }
		VALUE push(VALUE val);
		VALUE pop();
		VALUE unshift(VALUE val);
		VALUE shift();
		
		VALUE& operator[](int64_t index);
		VALUE operator[](int64_t index) const;

		static Array* copy(VALUE* data, size_t len);
		static Array* copy(const Array&);
		static Array* reference(VALUE* data, size_t len);
		static Array* reference(const Array&);
	};
}

#endif /* end of include guard: ARRAY_H_SCRHOXUE */
