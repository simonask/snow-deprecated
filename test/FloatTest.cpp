#include "test.h"
#include "runtime/Runtime.h"
#include <cmath>

using namespace snow;

TEST_SUITE(Float);

static const float a = 45.6;
static const float b = 78.0;

TEST_CASE(addition) {
	float correct = a + b;
	Value result = snow::call_method(value(a), "+", value(b));
	TEST_EQ(floatnum(result.value()), correct);
}

TEST_CASE(subtraction) {
	float correct = a - b;
	Value result = snow::call_method(value(a), "-", value(b));
	TEST_EQ(floatnum(result.value()), correct);
}

TEST_CASE(multiplication) {
	float correct = a * b;
	Value result = snow::call_method(value(a), "*", value(b));
	TEST_EQ(floatnum(result.value()), correct);
}

TEST_CASE(division) {
	float correct = a / b;
	Value result = snow::call_method(value(a), "/", value(b));
	TEST_EQ(floatnum(result.value()), correct);
}

TEST_CASE(modulus) {
	float correct = fmod(a,b);
	Value result = snow::call_method(value(a), "%", value(b));
	TEST_EQ(floatnum(result.value()), correct);
}

TEST_CASE(power) {
	float correct = pow(a,b);
	Value result = snow::call_method(value(a), "**", value(b));
	TEST_EQ(floatnum(result.value()), correct);
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

TEST_CASE(to_i) {
	int correct = (int)(a + b);
	Value added = snow::call_method(value(a), "+", value(b));
	Value result = snow::call_method(added, "to_i");
	TEST_EQ(integer(result.value()), correct);
}

TEST_CASE(to_f) {
	Value result = snow::call_method(value(a), "to_f");
	TEST_EQ(floatnum(result.value()), a);
}