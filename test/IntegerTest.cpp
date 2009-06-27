#include "test.h"
#include "runtime/Runtime.h"
#include <cmath>

using namespace snow;

TEST_SUITE(Integer);

static const intx a = 22;
static const intx b = 7;

TEST_CASE(addition) {
	intx correct = a + b;
	Value result = snow::call_method(value(a), "+", value(b));
	TEST_EQ(integer(result.value()), correct);
}

TEST_CASE(subtraction) {
	intx correct = a - b;
	Value result = snow::call_method(value(a), "-", value(b));
	TEST_EQ(integer(result.value()), correct);
}

TEST_CASE(multiplication) {
	intx correct = a * b;
	Value result = snow::call_method(value(a), "*", value(b));
	TEST_EQ(integer(result.value()), correct);
}

TEST_CASE(division) {
	intx correct = a / b;
	Value result = snow::call_method(value(a), "/", value(b));
	TEST_EQ(integer(result.value()), correct);
}

TEST_CASE(modulus) {
	intx correct = a % b;
	Value result = snow::call_method(value(a), "%", value(b));
	TEST_EQ(integer(result.value()), correct);
}

TEST_CASE(power) {
	#ifdef ARCH_IS_64_BIT
	intx correct = pow(a,b);
	Value result = snow::call_method(value(a), "**", value(b));
	#else
	intx correct = pow(5, 5);
	Value result = snow::call_method(value(5), "**", value(5));
	#endif
	TEST_EQ(integer(result.value()), correct);
}

TEST_CASE(equals) {
	bool correct = a == b;
	Value result = snow::call_method(value(a), "=", value(b));
	TEST_EQ(boolean(result.value()), correct);
}

TEST_CASE(less_than) {
	Value result = snow::call_method(value(a), "<", value(b));
	TEST_EQ(eval_truth(result.value()), a < b);
	result = snow::call_method(value(b), "<", value(a));
	TEST_EQ(eval_truth(result.value()), b < a);
}

TEST_CASE(less_than_or_equal) {
	bool correct = a <= b;
	Value result = snow::call_method(value(a), "<=", value(b));
	TEST_EQ(boolean(result.value()), correct);
}

TEST_CASE(greater_than) {
	bool correct = a > b;
	Value result = snow::call_method(value(a), ">", value(b));
	TEST_EQ(boolean(result.value()), correct);
}

TEST_CASE(greater_than_or_equal) {
	bool correct = a >= b;
	Value result = snow::call_method(value(a), ">=", value(b));
	TEST_EQ(boolean(result.value()), correct);
}

TEST_CASE(to_f) {
	float correct = 22.0;
	Value result = snow::call_method(value(a), "to_f");
	TEST_EQ(floatnum(result.value()), correct);
}

TEST_CASE(to_i) {
	Value result = snow::call_method(value(a), "to_i");
	TEST_EQ(integer(result.value()), a);
}