#ifndef ARRAY_H_SCRHOXUE
#define ARRAY_H_SCRHOXUE

#include "Object.h"
#include <vector>

namespace snot {
	Object* array_prototype();
	
	class Array : public Object {
	private:
		std::vector<VALUE> m_Values;
	public:
		Array(const Array& other);
		VALUE copy() const { return value(new Array(*this)); }
		
		std::vector<VALUE>& values() { return m_Values; }
		const std::vector<VALUE>& values() const { return m_Values; }
	};
}

#endif /* end of include guard: ARRAY_H_SCRHOXUE */
