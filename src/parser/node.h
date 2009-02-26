#ifndef NODE_H
#define NODE_H

#include <string>

namespace snow {
    
    struct Node {
    	virtual ~Node() {}
    	virtual std::string describe() {}
    };

    struct Literal : Node {
    	enum Type {
    		STRING_TYPE,
    		INTEGER_TYPE,
    		INTEGER_DEC_TYPE = INTEGER_TYPE,
    		INTEGER_HEX_TYPE,
    		INTEGER_BIN_TYPE,
    		FLOAT_TYPE,
    	};
    	std::string string;
    	Type type;

    	Literal(const std::string& str, Type type) : string(str), type(type) {}
        std::string describe() { return string; }
    };
    
}

#endif