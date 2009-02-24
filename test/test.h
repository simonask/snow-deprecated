#ifndef TEST_H_W9V78SBL
#define TEST_H_W9V78SBL

#include "Basic.h"
#include "Util.h"

#define TEST_SUITE_STR(SUITE) ("" #SUITE)

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
		Case(const char* name, CaseFunction func) : m_Name(name), m_Function(func), m_Next(NULL) {
			#ifdef TEST_SUITE
			m_SuiteName = TEST_SUITE;
			#else
			m_SuiteName = "<none>";
			#endif
			add(this);
		}
		
		int run();
		
		static int run_all(int argc, char** argv);
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
}
}

#define _SNOW_TEST_CASE_FUNC_NAME(NAME) _SNOW_TEST_ ## NAME ## _FUNC
#define _SNOW_TEST_CASE_NAME(NAME) _SNOW_TEST_ ## NAME ## _CASE

#define TEST_CASE(NAME) \
	static void _SNOW_TEST_CASE_FUNC_NAME(NAME)(); \
	static snow::test::Case _SNOW_TEST_CASE_NAME(NAME) = snow::test::Case(#NAME, _SNOW_TEST_CASE_FUNC_NAME(NAME)); \
	void _SNOW_TEST_CASE_FUNC_NAME(NAME)()
	
#define FAIL(MSG) \
	{throw(snow::test::TestFailure(MSG, __FILE__, __LINE__)); }
	
#define TEST(EXPR) \
	if (!(EXPR)) FAIL(#EXPR);

#define TEST_EQ(X, Y) \
	if ((X) != (Y)) FAIL(snow::string_printf("Expected " #X " == " #Y ", got %s != %s", (X), (Y)));
	
#define TEST_NIL(X) \
	if ((X) != nil()) FAIL(snow::string_printf("Expected nil from " #X ", got %s", (X)));
	
#define TEST_TRUTH(X) \
	if (!eval_truth(X)) FAIL(snow::string_printf("Expected truth from " #X ", got %s", (X)));
	
#define TEST_FALSE(X) \
	if (eval_truth(X)) FAIL(snow::string_printf("Expected false or nil from " #X ", got %s", (X)));

#endif /* end of include guard: TEST_H_W9V78SBL */
