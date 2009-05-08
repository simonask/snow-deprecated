#include "test.h"
#include "runtime/Runtime.h"

using namespace snow;

TEST_SUITE(Float);

static const float a = 45.6;
static const float b = 78.0;

TEST_CASE(add) {
	float correct = a + b;
	VALUE result = snow::call_method(value(a), "+", 1, value(b));
	TEST_EQ(floatnum(result), correct);
}

TEST_CASE(sub) {
	float correct = a - b;
	VALUE result = snow::call_method(value(a), "-", 1, value(b));
	TEST_EQ(floatnum(result), correct);
}

TEST_CASE(mul) {
	float correct = a * b;
	VALUE result = snow::call_method(value(a), "*", 1, value(b));
	TEST_EQ(floatnum(result), correct);
}

TEST_CASE(div) {
	float correct = a / b;
	VALUE result = snow::call_method(value(a), "/", 1, value(b));
	TEST_EQ(floatnum(result), correct);
}

TEST_CASE(to_i) {
	int correct = (int)(a + b);
	VALUE added = snow::call_method(value(a), "+", 1, value(b));
	VALUE result = snow::call_method(added, "to_i", 0);
	TEST_EQ(integer(result), correct);
}
