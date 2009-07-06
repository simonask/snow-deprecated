#ifndef ARRAY_H_SCRHOXUE
#define ARRAY_H_SCRHOXUE

#include "Object.h"
#include <vector>

namespace snow {
	Ptr<Object> array_prototype();
	void array_prototype_init();
	
	class Array : public Object {
	private:
		GC_ROOTS;

		DataPtr<VALUE> m_Data;
		uintx m_Length;
		uintx m_AllocatedLength;
		
		void resize(uintx new_length);

		Array(const Array&) = delete;
	public:
		NO_INIT;

		Array();
		
		Ptr<IObject> copy() const { return Array::copy(*this); }

		virtual Value call(const Value& self, const Arguments&);
		
		void set_data(const DataPtr<VALUE>& data, uintx len);
		void set_reference(const DataPtr<VALUE>& data, uintx len);
		void preallocate(uintx length);
		uintx length() const { return m_Length; }
		uintx allocated_length() const { return m_AllocatedLength; }
		DataPtr<VALUE> data() const { return m_Data; }
		
		Value get_by_index(intx index) const { return (*this)[index]; }
		Value set_by_index(intx index, const Value& val) { return (*this)[index] = val.value(); }
		Value push(const Value& val);
		Value pop();
		Value unshift(const Value& val);
		Value shift();
		Value delete_at(uintx index);
		
		VALUE& operator[](intx index);
		Value operator[](intx index) const;

		static Ptr<Array> copy(const DataPtr<VALUE>& data, uintx len);
		static Ptr<Array> copy(const Array&);
		static Ptr<Array> reference(const DataPtr<VALUE>& data, uintx len);
		static Ptr<Array> reference(const Array&);
	};
}

#endif /* end of include guard: ARRAY_H_SCRHOXUE */
