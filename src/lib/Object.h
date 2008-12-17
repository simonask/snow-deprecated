#ifndef OBJECT_H_ZOHWWN1D
#define OBJECT_H_ZOHWWN1D

#include <map>
#include <string>

#include "Handle.h"
#include "Function.h"
#include "Basic.h"

namespace snot {
class Object {
private:
	uint64_t m_Scope;
	std::map<std::string, Handle<Object> > m_InstanceVars;
public:
	
};
}


#endif /* end of include guard: OBJECT_H_ZOHWWN1D */
