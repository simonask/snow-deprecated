#include "test.h"
#include "runtime/RegEx.h"
using namespace snow;

TEST_SUITE(RegEx);

TEST_CASE(simple_match) {
	RegEx* rx = gc_new<RegEx>(gc_new<String>("h[a|e]j"));
	String* target1 = gc_new<String>("hej");
	String* target2 = gc_new<String>("haj");
	String* target3 = gc_new<String>("hoj");
	TEST_NEQ(rx->search(target1), NULL);
	TEST_NEQ(rx->search(target2), NULL);
	TEST_EQ(rx->search(target3), NULL);
}

