#include "SymbolTable.h"
#include <iostream>
using namespace std;

namespace snot {
Symbol define_symbol(SymbolTable& table, const std::string& name, const Symbol& symb) {
	if (table.find(name) != table.end()) {
		cerr << "WARNING: Symbol `" << name << "' is already defined in table 0x" << hex << &table << ", overwriting..." << endl;
	}
	table[name] = symb;
	return symb;
}
}

