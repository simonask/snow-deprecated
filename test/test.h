#ifndef TEST_H_W9V78SBL
#define TEST_H_W9V78SBL

#include "base/Basic.h"
#include "base/Util.h"

namespace snow {
namespace test {
	typedef void(*CaseFunction)();
	
	class Case {
	private:
		const char* m_SuiteName;
		const char* m_Name;
		const CaseFunction m_Function;
		Case* m_Next;
		
		static void add(Case*);
	public:
		Case(const char* name, CaseFunction func);
		
		int run();
		
		static int run_all(int argc, char** argv);
		
		static void __set_suite_name(const char*);
	};
	
	class TestFailure {
	private:
		std::string m_Expression;
		std::string m_File;
		int m_Line;
	public:
		TestFailure(const std::string& expr, const std::string& file, int line) : m_Expression(expr), m_File(file), m_Line(line) {}
		std::string expression() const { return m_Expression; }
		std::string file() const { return m_File; }
		int line() const { return m_Line; }
	};
	
	class TestPending {
	public:
		TestPending() {}
	};
}
}

#define _SNOW_TEST_CASE_FUNC_NAME(NAME) _SNOW_TEST_ ## NAME ## _FUNC
#define _SNOW_TEST_CASE_NAME(NAME) _SNOW_TEST_ ## NAME ## _CASE
#define _SNOW_TEST_SUITE_NAME(NAME) _SNOW_TEST_ ## NAME ## _SUITE
#define _SNOW_TEST_SUITE_NAME_INST(NAME) _SNOW_TEST_ ## NAME ## _SUITE_INST

#define TEST_SUITE(NAME) \
	static struct _SNOW_TEST_SUITE_NAME(NAME) { _SNOW_TEST_SUITE_NAME(NAME)() { snow::test::Case::__set_suite_name(#NAME); } } _SNOW_TEST_SUITE_NAME_INST(NAME);

#define TEST_CASE(NAME) \
	static void _SNOW_TEST_CASE_FUNC_NAME(NAME)(); \
	static snow::test::Case _SNOW_TEST_CASE_NAME(NAME) = snow::test::Case(#NAME, _SNOW_TEST_CASE_FUNC_NAME(NAME)); \
	void _SNOW_TEST_CASE_FUNC_NAME(NAME)()

#define PENDING_TEST_CASE(NAME) \
	TEST_CASE(NAME) PENDING()

#define FAIL(MSG) \
	{throw(snow::test::TestFailure(MSG, __FILE__, __LINE__)); }
	
#define PENDING() \
	{ throw(snow::test::TestPending()); }
	
#define TEST(EXPR) \
	if (!(EXPR)) FAIL(#EXPR);
	
#define TEST_EQ(X, Y) \
	if ((X) != (Y)) FAIL(snow::string_printf("Expected " #X " == " #Y ", got %s != %s", (X), (Y)));

#define TEST_NEQ(X, Y) \
	if ((X) == (Y)) FAIL(snow::string_printf("Expected " #X " != " #Y ));
	
#define TEST_NIL(X) \
	if ((X) != nil()) FAIL(snow::string_printf("Expected nil from " #X ", got %s", (X)));
	
#define TEST_TRUTH(X) \
	if (!eval_truth(X)) FAIL(snow::string_printf("Expected truth from " #X ", got %s", (X)));
	
#define TEST_FALSE(X) \
	if (eval_truth(X)) FAIL(snow::string_printf("Expected false or nil from " #X ", got %s", (X)));

#endif /* end of include guard: TEST_H_W9V78SBL */
