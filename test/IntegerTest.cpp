#include "test.h"
#include "runtime/Runtime.h"
#include <cmath>

using namespace snow;

TEST_SUITE(Integer);

static const intx a = 22;
static const intx b = 7;

TEST_CASE(addition) {
	intx correct = a + b;
	VALUE result = snow::call_method(value(a), "+", 1, value(b));
	TEST_EQ(integer(result), correct);
}

TEST_CASE(subtraction) {
	intx correct = a - b;
	VALUE result = snow::call_method(value(a), "-", 1, value(b));
	TEST_EQ(integer(result), correct);
}

TEST_CASE(multiplication) {
	intx correct = a * b;
	VALUE result = snow::call_method(value(a), "*", 1, value(b));
	TEST_EQ(integer(result), correct);
}

TEST_CASE(division) {
	intx correct = a / b;
	VALUE result = snow::call_method(value(a), "/", 1, value(b));
	TEST_EQ(integer(result), correct);
}

TEST_CASE(modulus) {
	intx correct = a % b;
	VALUE result = snow::call_method(value(a), "%", 1, value(b));
	TEST_EQ(integer(result), correct);
}

TEST_CASE(power) {
	#ifdef ARCH_IS_64_BIT
	intx correct = pow(a,b);
	VALUE result = snow::call_method(value(a), "**", 1, value(b));
	#else
	intx correct = pow(5, 5);
	VALUE result = snow::call_method(value(5), "**", 1, value(5));
	#endif
	TEST_EQ(integer(result), correct);
}

TEST_CASE(equals) {
	bool correct = a == b;
	VALUE result = snow::call_method(value(a), "=", 1, value(b));
	TEST_EQ(boolean(result), correct);
}

TEST_CASE(less_than) {
	VALUE result = snow::call_method(value(a), "<", 1, value(b));
	TEST_EQ(eval_truth(result), a < b);
	result = snow::call_method(value(b), "<", 1, value(a));
	TEST_EQ(eval_truth(result), b < a);
}

TEST_CASE(less_than_or_equal) {
	bool correct = a <= b;
	VALUE result = snow::call_method(value(a), "<=", 1, value(b));
	TEST_EQ(boolean(result), correct);
}

TEST_CASE(greater_than) {
	bool correct = a > b;
	VALUE result = snow::call_method(value(a), ">", 1, value(b));
	TEST_EQ(boolean(result), correct);
}

TEST_CASE(greater_than_or_equal) {
	bool correct = a >= b;
	VALUE result = snow::call_method(value(a), ">=", 1, value(b));
	TEST_EQ(boolean(result), correct);
}

TEST_CASE(to_f) {
	float correct = 22.0;
	VALUE result = snow::call_method(value(a), "to_f", 0);
	TEST_EQ(floatnum(result), correct);
}

TEST_CASE(to_i) {
	VALUE result = snow::call_method(value(a), "to_i", 0);
	TEST_EQ(integer(result), a);
}