#ifndef FLOAT_CPP_71JI96WU
#define FLOAT_CPP_71JI96WU

#include "base/Basic.h"
#include "Object.h"

namespace snow {
	Ptr<Object> float_prototype();
	
	#ifdef ARCH_IS_32_BIT
	class Float : public ThinObject
	{
	private:
		float m_Value;
	public:
		NO_INIT;
		
		Float(float val = 0.f) : ThinObject(float_prototype()), m_Value(val) {}
		Ptr<IObject> copy() const { return const_cast<Float*>(this); }
		
		float value() const { return m_Value; }
	};
	#endif
}

#endif /* end of include guard: FLOAT_CPP_71JI96WU */
