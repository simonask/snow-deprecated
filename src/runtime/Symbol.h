#ifndef SYMBOL_H_HJ32LKJ
#define SYMBOL_H_HJ32LKJ

#include "runtime/Value.h"
#include "runtime/Handle.h"

namespace snow {
	class Object;

	Ptr<Object> symbol_prototype();
	
	class Symbol {
	private:
		uintx m_SymbolIndex;
	public:
		Symbol(const char* str);
		Symbol(const Symbol& other) : m_SymbolIndex(other.m_SymbolIndex) {}
		Symbol(const Value& val);
		Symbol(VALUE val);
		
		operator Value() const;
		operator VALUE() const;
		
		bool operator==(const Symbol& other) const { return m_SymbolIndex == other.m_SymbolIndex; }
		bool operator!=(const Symbol& other) const { return m_SymbolIndex != other.m_SymbolIndex; }
	};

	inline Symbol::operator Value() const {
		Value val;
		val = (VALUE)*this;
		return val;
	}
}

#endif // SYMBOL_H_HJ32LKJ

