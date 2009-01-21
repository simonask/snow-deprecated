#include "Integer.h"
#include "Runtime.h"

namespace snow {
	static Object* IntegerPrototype = NULL;
	
	static VALUE integer_puts(VALUE self, uint64_t num_args, VALUE*) {
		int64_t n = integer(self);
		printf("%lld\n", n);
		return nil();
	}
	
	static VALUE integer_add(VALUE self, uint64_t num_args, VALUE* args) {
		if (num_args == 0)
			return self;
		int64_t a = integer(self);
		int64_t b = integer(args[0]);
		return value(a + b);
	}
	
	Object* integer_prototype() {
		if (IntegerPrototype)
			return IntegerPrototype;
		Object* ip = new Object;
		ip->set("name", create_string("Integer"));
		ip->set("puts", create_function(integer_puts));
		ip->set("+", create_function(integer_add));
		IntegerPrototype = ip;
		return IntegerPrototype;
	}
}
