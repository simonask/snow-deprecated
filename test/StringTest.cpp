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

TEST_CASE(addition) {
	std::stringstream ss;
	ss << a << b;
	std::string correct = ss.str();
	
	VALUE self = create_string(a.c_str());
	VALUE arg = create_string(b.c_str());
	auto result = object_cast<String>(snow::call(self, get(self, "+"), 1, arg));
	TEST_EQ(result->str(), ss.str());
}