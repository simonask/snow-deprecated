#include "test.h"
#include "parser/Driver.h"
#include <cmath>

using namespace snow;

TEST_SUITE(ParserExpression);

TEST_CASE(addition) {
    snow::Driver driver;
    int64_t correct = 19+213;
    VALUE v = driver.parse_string("19+213");
    TEST_EQ(integer(v), correct);
}

TEST_CASE(subtraction) {
    snow::Driver driver;
    int64_t correct = 19-213;
    VALUE v = driver.parse_string("19-213");
    TEST_EQ(integer(v), correct);
}

TEST_CASE(unary_minus) {
    snow::Driver driver;
    int64_t correct = -19;
    VALUE v = driver.parse_string("-19");
    TEST_EQ(integer(v), correct);
}

TEST_CASE(multiplication) {
    snow::Driver driver;
    int64_t correct = 19*213;
    VALUE v = driver.parse_string("19*213");
    TEST_EQ(integer(v), correct);
}

TEST_CASE(division) {
    snow::Driver driver;
    int64_t correct = 22/7;
    VALUE v = driver.parse_string("22/7");
    TEST_EQ(integer(v), correct);
}

TEST_CASE(modulus) {
    snow::Driver driver;
    int64_t correct = 22 % 7;
    VALUE v = driver.parse_string("22 % 7");
    TEST_EQ(integer(v), correct);
}

TEST_CASE(power) {
    snow::Driver driver;
    int64_t correct = pow(9,4);
    VALUE v = driver.parse_string("9 ** 4");
    TEST_EQ(integer(v), correct);
}