#include "test.h"
#include "Kernel.h"

using namespace snow;

TEST_SUITE(Parser);

TEST_CASE(assignment) {
    int64_t correct = 3;
    VALUE v = Kernel::eval("h: 3");
    TEST_EQ(integer(v), correct);
}

TEST_CASE(function) {
    int64_t correct = 26;
    VALUE v = Kernel::eval("double: [a] { a*2 }; double(12)+2");
    TEST_EQ(integer(v), correct);
}