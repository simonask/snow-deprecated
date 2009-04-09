#include "Symbol.h"
#include "Array.h"
#include "SnowString.h"

#define FUNC(name) (new(kMalloc) Function(name))

namespace snow {
	static inline VALUE symbol_index_to_value(uint64_t index) {
		return reinterpret_cast<VALUE>((index << 8) | kSymbolType);
	}

	static inline uint64_t symbol_value_to_index(VALUE sym) {
		return reinterpret_cast<uint64_t>(sym) >> 8;
	}

	typedef std::vector<String*> SymbolList;

	static SymbolList& symbol_list() {
		static SymbolList* list = NULL;
		if (list)
			return *list;
		list = new SymbolList;
		return *list;
	}

	VALUE symbol(const char* _str) {
		// Find existing symbol, if it exists...
		String* str = new(kMalloc) String(std::string(_str));
		for (size_t i = 0; i < symbol_list().size(); ++i) {
			String* current = object_cast<String>(symbol_list()[i]);
			ASSERT(current);
			if (*current == *str)
				return symbol_index_to_value(i);
		}

		// Not found, add it...
		VALUE sym = symbol_index_to_value(symbol_list().size());
		symbol_list().push_back(str);
		return sym;
	}

	static VALUE symbol_to_string(VALUE self, uint64_t, VALUE*) {
		ASSERT(is_symbol(self));
		uint64_t index = symbol_value_to_index(self);
		VALUE str = symbol_list()[index];
		ASSERT(!is_nil(str));
		return str;
	}

	static VALUE symbol_inspect(VALUE self, uint64_t, VALUE*) {
		ASSERT(is_symbol(self));
		VALUE str = symbol_to_string(self, 0, NULL);
		return snow::call_method(new String("#"), "+", 1, str);
	}
	
	Object* symbol_prototype() {
		static Object* proto = NULL;
		if (proto)
			return proto;

		proto = new(kMalloc) Object;
		proto->set_by_string("to_string", FUNC(symbol_to_string));
		proto->set_by_string("inspect", FUNC(symbol_inspect));
		return proto;
	}
}
