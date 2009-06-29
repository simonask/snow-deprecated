#include "Symbol.h"
#include "Array.h"
#include "SnowString.h"
#include "Function.h"

namespace snow {
	static inline VALUE symbol_index_to_value(uintx index) {
		return reinterpret_cast<VALUE>((index << 8) | kSymbolType);
	}

	static inline uintx symbol_value_to_index(VALUE sym) {
		ASSERT(is_symbol(sym));
		return reinterpret_cast<uintx>(sym) >> 8;
	}

	typedef std::vector<Ptr<String>> SymbolList;

	static SymbolList& symbol_list() {
		static SymbolList* list = NULL;
		if (list)
			return *list;
		list = new SymbolList;
		return *list;
	}
	
	static uintx symbol_index(const char* _str) {
		// Find existing symbol, if it exists...
		Ptr<String> str = gc_new<String>(_str);
		for (size_t i = 0; i < symbol_list().size(); ++i) {
			Ptr<String> current = object_cast<String>(symbol_list()[i]);
			ASSERT(current);
			if (*current == *str)
				return i;
		}

		// Not found, add it...
		uintx sym = symbol_list().size();
		symbol_list().push_back(str);
		return sym;
	}

	Symbol::Symbol(const char* str) : m_SymbolIndex(symbol_index(str)) {}
	
	Symbol::Symbol(const Value& val) : m_SymbolIndex(symbol_value_to_index(val.value())) {}
	Symbol::Symbol(VALUE val) : m_SymbolIndex(symbol_value_to_index(val)) {}
	
	Symbol::operator VALUE() const {
		return symbol_index_to_value(m_SymbolIndex);
	}

	static Value symbol_to_string(const Value& self, const Arguments& args) {
		ASSERT(self.is_symbol());
		uintx index = symbol_value_to_index(self.value());
		Value str = symbol_list()[index];
		ASSERT(!str.is_nil());
		return str;
	}

	static Value symbol_inspect(const Value& self, const Arguments& args) {
		ASSERT(self.is_symbol());
		Value str = symbol_to_string(self, args);
		return snow::call_method(gc_new<String>("#"), "+", str);
	}
	
	Ptr<Object> symbol_prototype() {
		static Ptr<Object> proto;
		if (proto)
			return proto;

		proto = malloc_new<Object>();
		proto->set_raw("to_string", gc_new<Function>(symbol_to_string));
		proto->set_raw("inspect", gc_new<Function>(symbol_inspect));
		return proto;
	}
	
	void _debug_print_symbol(const Symbol& sym) {
		debug(value_to_string(sym));
	}
}
