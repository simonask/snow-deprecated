#ifndef ARRAY_H_SCRHOXUE
#define ARRAY_H_SCRHOXUE

#include "Object.h"
#include <vector>

namespace snow {
	Handle<Object>& array_prototype();
	
	class Array : public Object {
	private:
		VALUE* m_Data;
		size_t m_Length;
		size_t m_AllocatedSize;
		
		Garbage::Lock m_GCLock;
		
		void resize(size_t new_size);
		void ensure_length(size_t length);
	public:
		explicit Array(size_t preallocate_length = 0);
		Array(VALUE* existing_array, size_t len, bool copy = true);
		Array(const Array& other);
		
		virtual void gc_mark();
		
		virtual void unfreeze() { Object::unfreeze(); resize(m_AllocatedSize); }
		
		VALUE copy() const { return value(new Array(*this)); }
		
		size_t length() const { return m_Length; }
		size_t allocated_size() const { return m_AllocatedSize; }
		VALUE* data() const { return m_Data; }
		
		VALUE get_by_index(int64_t index) const { return (*this)[index]; }
		VALUE set_by_index(int64_t index, VALUE val) { return (*this)[index] = val; }
		VALUE push(VALUE val);
		VALUE pop();
		VALUE unshift(VALUE val);
		VALUE shift();
		
		VALUE& operator[](int64_t index);
		VALUE operator[](int64_t index) const;
	};
}

#endif /* end of include guard: ARRAY_H_SCRHOXUE */
