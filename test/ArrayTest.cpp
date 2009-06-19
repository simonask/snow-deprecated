#include "test.h"
#include "runtime/Array.h"
using namespace snow;

TEST_SUITE(Array);

TEST_CASE(push) {
	Array* a = gc_new<Array>();
	VALUE val = value(45);
	
	TEST(a->length() == 0);
	a->push(val);
	TEST(a->length() == 1);
	TEST(a->get_by_index(0) == val);
	TEST((*a)[0] == val);
}

TEST_CASE(pop) {
	Array* a = gc_new<Array>();
	
	a->push(value(23));
	a->push(value(56));
	TEST(a->length() == 2);
	VALUE v = a->pop();
	TEST(a->length() == 1);
	TEST(v == value(56));
}

TEST_CASE(unshift) {
	Array* a = gc_new<Array>();
	a->unshift(value(56));
	a->unshift(value(98));
	a->unshift(value(123456));
	TEST(a->length() == 3);
	TEST_EQ((*a)[0], value(123456));
	TEST_EQ((*a)[1], value(98));
	TEST_EQ((*a)[2], value(56));
}

TEST_CASE(shift) {
	Array* a = gc_new<Array>();
	a->push(value(23));
	a->push(value(78));
	a->push(value(2));
	TEST(a->length() == 3);
	VALUE val = a->shift();
	TEST(val == value(23));
	TEST(a->length() == 2);
	TEST((*a)[0] == value(78));
	TEST((*a)[1] == value(2));
}

TEST_CASE(copy_c_array_on_write) {
	VALUE vals[] = {value(23), value(87), value(12)};
	
	Array* a = Array::reference(vals, 3);
	TEST(a->length() == 3);
	TEST(a->data() == vals);
	a->push(value(234));
	TEST(a->length() == 4);
	TEST(a->data() != vals);
}

TEST_CASE(automatic_resize) {
	Array* _a = gc_new<Array>();
	Array& a(*_a);
	
	a[0] = value(123);
	a[4] = value(456);
	TEST_EQ(a.length(), 5);
	TEST_EQ(a[0], value(123));
	TEST_NIL(a[1]);
	TEST_NIL(a[2]);
	TEST_NIL(a[3]);
	TEST_EQ(a[4], value(456));
}
