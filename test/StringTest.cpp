#include <string>
#include <sstream>
#include <iostream>
#include "test.h"
#include "runtime/Runtime.h"
#include "runtime/SnowString.h"

using namespace snow;

TEST_SUITE(String);

static const std::string a("foo");
static const std::string b("bar");

TEST_CASE(concatenation) {
	HandleScope _s;
	std::stringstream ss;
	ss << a << b;
	std::string correct = ss.str().c_str();
	
	Value self = gc_new<String>(a.c_str());
	Value arg = gc_new<String>(b.c_str());
	
	auto res1 = object_cast<String>(snow::call_method(self, "+", arg));
	TEST_EQ(*res1, *gc_new<String>(ss.str()));
	
	auto res2 = object_cast<String>(snow::call_method(self, "<<", arg));
	TEST_EQ(*res2, *res1);
}

TEST_CASE(reverse) {
	HandleScope _s;
	Handle<String> self = gc_new<String>(a.c_str());
	Handle<String> correct = gc_new<String>("oof");
	Handle<String> result = object_cast<String>(snow::call_method(self, "reverse"));
	TEST_EQ(*result, *correct);
}

TEST_CASE(substring) {
	HandleScope _s;
	Handle<String> a = gc_new<String>("The brown fox jumps over the railing, to its certain death.");
	Handle<String> correct = gc_new<String>("brown fox");
	Handle<String> b = a->substring_p(4, 9);
	TEST_EQ(*correct, *b);
}

TEST_CASE(length) {
	HandleScope _s;
	Handle<String> empty           = gc_new<String>("");
	Handle<String> three_letters   = gc_new<String>("ddd");
	Handle<String> fifteen_letters = gc_new<String>("123456789012345");
	
	TEST_EQ(snow::get(empty,           "length"), value(0));
	TEST_EQ(snow::get(three_letters,   "length"), value(3));
	TEST_EQ(snow::get(fifteen_letters, "length"), value(15));
}
