#include "test.h"
#include "runtime/Array.h"
using namespace snow;

TEST_SUITE(Array);

TEST_CASE(push) {
	Array* a = new Array;
	VALUE val = value(45LL);
	
	TEST(a->length() == 0);
	a->push(val);
	TEST(a->length() == 1);
	TEST(a->get_by_index(0) == val);
	TEST((*a)[0] == val);
}

TEST_CASE(pop) {
	Array* a = new Array;
	
	a->push(value(23LL));
	a->push(value(56LL));
	TEST(a->length() == 2);
	VALUE v = a->pop();
	TEST(a->length() == 1);
	TEST(v == value(56LL));
}

TEST_CASE(unshift) {
	Array* a = new Array;
	a->unshift(value(56LL));
	a->unshift(value(98LL));
	a->unshift(value(123456LL));
	TEST(a->length() == 3);
	TEST_EQ((*a)[0], value(123456LL));
	TEST_EQ((*a)[1], value(98LL));
	TEST_EQ((*a)[2], value(56LL));
}

TEST_CASE(shift) {
	Array* a = new Array;
	a->push(value(23LL));
	a->push(value(78LL));
	a->push(value(2LL));
	TEST(a->length() == 3);
	VALUE val = a->shift();
	TEST(val == value(23LL));
	TEST(a->length() == 2);
	TEST((*a)[0] == value(78LL));
	TEST((*a)[1] == value(2LL));
}

TEST_CASE(copy_c_array_on_write) {
	VALUE vals[] = {value(23LL), value(87LL), value(12LL)};
	
	Array* a = Array::reference(vals, 3);
	TEST(a->length() == 3);
	TEST(a->data() == vals);
	a->push(value(234LL));
	TEST(a->length() == 4);
	TEST(a->data() != vals);
}

TEST_CASE(automatic_resize) {
	Array* _a = new Array;
	Array& a(*_a);
	
	a[0] = value(123LL);
	a[4] = value(456LL);
	TEST_EQ(a.length(), 5);
	TEST_EQ(a[0], value(123LL));
	TEST_NIL(a[1]);
	TEST_NIL(a[2]);
	TEST_NIL(a[3]);
	TEST_EQ(a[4], value(456LL));
}
