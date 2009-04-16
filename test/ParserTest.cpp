#include "test.h"
#include "Kernel.h"
#include "parser/Driver.h"
#include "XMLCodegen.h"
#include <cmath>

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

TEST_CASE(long_conditionals) {
	int64_t correct = 3;
	VALUE v = Kernel::eval("i: 3; if i > 2; i; else; i: i * 2; end");
	TEST_EQ(integer(v), correct);
	
	correct = 6;
	v = Kernel::eval("i: 3; unless i > 2; i; else; i: i * 2; end");
	TEST_EQ(integer(v), correct);
}

TEST_CASE(short_conditionals) {
	int64_t correct = 3;
	VALUE v = Kernel::eval("i: 3; unless i > 2; return i * 2; end; i");
	TEST_EQ(integer(v), correct);
	
	correct = 6;
	v = Kernel::eval("i: 3; if i > 2; return i * 2; end; i");
	TEST_EQ(integer(v), correct);
}

TEST_CASE(inline_conditionals) {
	int64_t correct = 3;
	VALUE v = Kernel::eval("i: 3; return i * 2 unless i > 2; i");
	TEST_EQ(integer(v), correct);
	
	correct = 6;
	v = Kernel::eval("i: 3; return i * 2 if i > 2; i");
	TEST_EQ(integer(v), correct);
}

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

TEST_CASE(symbols) {
	VALUE correct1 = symbol("hej");
	VALUE v1 = Kernel::eval("#hej");
	
	VALUE correct2 = symbol("hej dav");
	VALUE v2 = Kernel::eval("#\"hej dav\"");
		
	TEST_EQ(v1, correct1);
	TEST_EQ(v2, correct2);
}

TEST_CASE(associativity) {
	int64_t correct = (5+4)*3;
	VALUE v = Kernel::eval("5+4*3");
	TEST_EQ(integer(v), correct);
}