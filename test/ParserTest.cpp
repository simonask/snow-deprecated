#include "test.h"
#include "parser/Driver.h"

using namespace snow;

TEST_SUITE(Parser);

TEST_CASE(assignment) {
    snow::Driver driver;
    int64_t correct = 3;
    VALUE v = driver.parse_string("h: 3");
    TEST_EQ(integer(v), correct);
}

TEST_CASE(function) {
    snow::Driver driver;
    int64_t correct = 26;
    VALUE v = driver.parse_string("double: [a] { a*2 }; double(12)+2");
    TEST_EQ(integer(v), correct);
}