#ifndef SYMBOL_H_HJ32LKJ
#define SYMBOL_H_HJ32LKJ

#include "Value.h"

namespace snow {
	class Object;

	Object* symbol_prototype();

	VALUE symbol(const char* name);
	const char* symbol_to_string(VALUE sym);
}

#endif // SYMBOL_H_HJ32LKJ
