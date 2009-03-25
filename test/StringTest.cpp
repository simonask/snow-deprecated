#include <string>
#include <sstream>
#include <iostream>
#include "test.h"
#include "lib/Runtime.h"
#include "lib/SnowString.h"

using namespace snow;

TEST_SUITE(String);

static const std::string a("foo");
static const std::string b("bar");

TEST_CASE(concatenation) {
	std::stringstream ss;
	ss << a << b;
	std::string correct = ss.str();
	
	VALUE self = create_string(a.c_str());
	VALUE arg = create_string(b.c_str());
	
	auto res1 = object_cast<String>(snow::call(self, get(self, "+"), 1, arg));
	TEST_EQ(res1->str(), ss.str());
	
	auto res2 = object_cast<String>(snow::call(self, get(self, "<<"), 1, arg));
	TEST_EQ(res2->str(), res1->str());
}

TEST_CASE(reverse) {
	VALUE self = create_string(a.c_str());
	std::string correct("oof");
	auto result = object_cast<String>(snow::call(self, get(self, "reverse"), 0));
	TEST_EQ(result->str(), correct);
}