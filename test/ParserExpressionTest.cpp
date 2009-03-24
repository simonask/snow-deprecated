#include "test.h"
#include "Kernel.h"
#include <cmath>

using namespace snow;

TEST_SUITE(ParserExpression);

TEST_CASE(addition) {
    int64_t correct = 19+213;
    VALUE v = Kernel::eval("19+213");
    TEST_EQ(integer(v), correct);
}

TEST_CASE(subtraction) {
    int64_t correct = 19-213;
    VALUE v = Kernel::eval("19-213");
    TEST_EQ(integer(v), correct);
}

TEST_CASE(unary_minus) {
    int64_t correct = -19;
    VALUE v = Kernel::eval("-19");
    TEST_EQ(integer(v), correct);
}

TEST_CASE(multiplication) {
    int64_t correct = 19*213;
    VALUE v = Kernel::eval("19*213");
    TEST_EQ(integer(v), correct);
}

TEST_CASE(division) {
    int64_t correct = 22/7;
    VALUE v = Kernel::eval("22/7");
    TEST_EQ(integer(v), correct);
}

TEST_CASE(modulus) {
    int64_t correct = 22 % 7;
    VALUE v = Kernel::eval("22 % 7");
    TEST_EQ(integer(v), correct);
}

TEST_CASE(power) {
    int64_t correct = pow(9,4);
    VALUE v = Kernel::eval("9 ** 4");
    TEST_EQ(integer(v), correct);
}

TEST_CASE(sqrt) {
    float correct = sqrt(22);
    VALUE v = Kernel::eval("22.sqrt()");
    TEST_EQ(floatnum(v), correct);
}