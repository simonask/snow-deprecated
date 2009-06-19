#include "test.h"
#include "runtime/Exception.h"
#include "base/Basic.h"
using namespace snow;

TEST_SUITE(Exception);

TEST_CASE(throw_catch) {
	HandleScope _scope;
	ExceptionHandler _handler;
	volatile int a = 0;
	volatile int b = 0;
	if (TRY_CATCH(_handler)) {
		a = 1;
		throw_exception(value(123));
		b = 1;
		// SHOULD NOT BE REACHED
		TEST_EQ(true, false);
	} else {
		// Exception caught
		TEST_EQ(_handler.exception(), value(123));
	}
	TEST_EQ(a, 1);
	TEST_EQ(b, 0);
}

struct DestructorsTestMock {
	volatile int* n;
	DestructorsTestMock(volatile int* m) : n(m) { (*n)++; }
	~DestructorsTestMock() { (*n)++; }
};

static void destructors_test_helper(volatile int* n) {
	HandleScope _s;
	Local<DestructorsTestMock> mock(n);
	throw_exception(nil());
}

TEST_CASE(destructors) {
	HandleScope _s;
	ExceptionHandler tc;
	volatile int a = 0;
	if (TRY_CATCH(tc)) {
		TEST_EQ(a, 0);
		destructors_test_helper(&a);
	} else {
		// if a == 1, the destructor wasn't called :(
		TEST_EQ(a, 2);
	}
}

