#include "Integer.h"
#include "Runtime.h"
#include "SnowString.h"
#include <sstream>
#include "InternalMacros.h"

namespace snow {
	static Handle<Object> IntegerPrototype = NULL;
	
	static VALUE integer_puts(Scope* scope) {
		int64_t n = integer(SELF);
		printf("%lld\n", n);
		return nil();
	}
	
	static VALUE integer_add(Scope* scope) {
		if (NUM_ARGS == 0)
			return SELF;
		int64_t a = integer(SELF);
		int64_t b = integer(ARGS[0]);
		return value(a + b);
	}
	
	static VALUE integer_to_string(Scope* scope) {
		std::stringstream ss;
		ss << integer(SELF);
		return value(new String(ss.str()));
	}
	
	static VALUE integer_less(Scope* scope) {
		ASSERT_ARGS(NUM_ARGS >= 1);
		return value(SELF < ARGS[0]);
	}
	
 	Handle<Object>& integer_prototype() {
		if (IntegerPrototype)
			return IntegerPrototype;
		Object* ip = new Object(object_prototype());
		ip->set("name", create_string("Integer"));
		ip->set("puts", create_function(integer_puts));
		ip->set("+", create_function(integer_add));
		ip->set("to_string", create_function(integer_to_string));
		ip->set("<", create_function(integer_less));
		IntegerPrototype = ip;
		return IntegerPrototype;
	}
}
