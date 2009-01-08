#include "SymbolTable.h"

namespace snow {
Symbol define_symbol(SymbolTable& table, const std::string& name, const Symbol& symb) {
	if (table.find(name) != table.end()) {
		warn("Symbol `%s' is already defined in table 0x%lx, overwriting...\n", name.c_str(), &table);
	}
	table.insert(SymbolTable::value_type(name, symb));
	return symb;
}
}

