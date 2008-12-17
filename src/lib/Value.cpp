#include "Value.h"
namespace snot {
	ValueType value_type(VALUE val) {
		if (is_object(val))
			return kObjectType;
		else if (is_undefined(val))
			return kUndefinedType;
		else if (is_boolean(val))
			return kBooleanType;
		else if (is_integer(val))
			return kIntegerType;
	}
}
