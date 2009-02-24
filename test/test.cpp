#include "test.h"
#include <stdexcept>
#include <sstream>

namespace snow { namespace test {
	static Case* first = NULL;
	
	static int passed_tests = 0;
	static int failed_tests = 0;
	static bool rethrow = false;
	
	void Case::add(Case* c) {
		static Case* last = NULL;
		Case* previous = last;
		if (previous) {
			previous->m_Next = c;
		}
		
		if (first == NULL) {
			first = c;
		}
		last = c;
	}
	
	int Case::run() {
		printf("%-20s%-40s", m_SuiteName, m_Name);
		
		std::string message;
		bool failed = true;
		try {
			m_Function();
			failed = false;
		}
		catch (const TestFailure& ex) {
			char* str;
			asprintf(&str, "Test failed at %s:%d:\t\t%s\n", ex.file().c_str(), ex.line(), ex.expression().c_str());
			message = str;
			free(str);
		}
		catch (const std::runtime_error& ex) {
			std::stringstream ss;
			ss << "STL exception thrown – enable rethrow to get a backtrace.\n";
			ss << "what(): " << ex.what();
			ss << '\n';
			message = ss.str();
			if (rethrow)
				throw(ex);
		}
		
		if (failed) {
			++failed_tests;
			printf("\x1b[1;31;40mFAILED\x1b[0;37;40m\n");
		} else {
			++passed_tests;
			printf("\x1b[1;32;40mOK\x1b[0;37;40m\n");
		}
		
		printf(message.c_str());
		
		return failed ? 1 : 0;
	}
	
	int Case::run_all(int argc, char** argv) {
		printf("\x1b[1;36;40m%-20s%-40s%s\x1b[0;37;40m\n", "SUITE", "CASE", "RESULT");
		Case* c = first;
		if (c == NULL)
			printf("No tests.\n");
		while (c) {
			c->run();
			c = c->m_Next;
		}
		printf("%d passed, %d failed (%d total)\n", passed_tests, failed_tests, passed_tests + failed_tests);
		return 0;
	}
}}