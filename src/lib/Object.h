#ifndef OBJECT_H_ZOHWWN1D
#define OBJECT_H_ZOHWWN1D

#include "Basic.h"
#include "Value.h"
#include <stdarg.h>
#include <map>
#include <string>

namespace snot {
class Object {
private:
	Object* m_Prototype;
public:
	Object(Object* prototype = NULL) : m_Prototype(prototype) {}
	virtual VALUE call() { return value(this); }
	VALUE send(const char* message, uint64_t num_args, ...);
	VALUE va_send(const char* message, uint64_t num_args, va_list& ap);
	
	Object* prototype() const { return m_Prototype; }
};
}

#endif /* end of include guard: OBJECT_H_ZOHWWN1D */
