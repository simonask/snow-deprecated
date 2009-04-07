#include "test.h"
#include "lib/Runtime.h"
#include <cmath>

using namespace snow;

TEST_SUITE(Integer);

static const int64_t a = 22;
static const int64_t b = 7;

TEST_CASE(addition) {
	int64_t correct = a + b;
	VALUE result = snow::call_method(value(a), "+", 1, value(b));
	TEST_EQ(integer(result), correct);
}

TEST_CASE(subtraction) {
	int64_t correct = a - b;
	VALUE result = snow::call_method(value(a), "-", 1, value(b));
	TEST_EQ(integer(result), correct);
}

TEST_CASE(multiplication) {
	int64_t correct = a * b;
	VALUE result = snow::call_method(value(a), "*", 1, value(b));
	TEST_EQ(integer(result), correct);
}

TEST_CASE(division) {
	int64_t correct = a / b;
	VALUE result = snow::call_method(value(a), "/", 1, value(b));
	TEST_EQ(integer(result), correct);
}

TEST_CASE(modulus) {
	int64_t correct = a % b;
	VALUE result = snow::call_method(value(a), "%", 1, value(b));
	TEST_EQ(integer(result), correct);
}

TEST_CASE(power) {
	int64_t correct = pow(a,b);
	VALUE result = snow::call_method(value(a), "**", 1, value(b));
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

TEST_CASE(square_root) {
	float correct = sqrt(a);
	VALUE result = snow::call_method(value(a), "sqrt", 0);
	TEST_EQ(floatnum(result), correct);
}
