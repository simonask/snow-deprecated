#include "test.h"
#include <stdexcept>
#include <sstream>
#include "runtime/Exception.h"
#include "runtime/StackFrame.h"

namespace snow { namespace test {
	static Case* first = NULL;
	static const char* current_suite = NULL;
	
	static int passed_tests = 0;
	static int failed_tests = 0;
	static int pending_tests = 0;
	static bool rethrow = false;
	
	Case::Case(const char* name, CaseFunction func) : m_Name(name), m_Function(func), m_Next(NULL) {
		m_SuiteName = current_suite ? current_suite : "<none>";
		add(this);
	}
	
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
		bool pending = false;
		
		HandleScope _;
		try {
			ExceptionHandler handler;
			if (TRY_CATCH(handler)) {
				m_Function();
				failed = false;
			} else {
				std::stringstream ss;
				ss << "Unhandled exception (Snow): " << value_to_string(handler.exception());
				ss << std::endl;
				const char* const* trace = handler.stack_trace();
				while (*trace) {
					ss << *trace << std::endl;
					++trace;
				}
				throw TestFailure(ss.str(), handler.stack_frame()->file, handler.stack_frame()->line);
			}
		}
		catch (const TestFailure& ex) {
			char* str;
			asprintf(&str, "Test failed at %s:%d:\t\t%s\n", ex.file().c_str(), ex.line(), ex.expression().c_str());
			message = str;
			free(str);
		}
		catch (const TestPending& ex) {
			failed = false;
			pending = true;
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
			printf("\x1b[1;31mFAILED\x1b[0m\n");
		} else if (pending) {
			++pending_tests;
			printf("\x1b[1;33mPENDING\x1b[0m\n");
		} else if (!failed && !pending) {
			++passed_tests;
			printf("\x1b[1;32mOK\x1b[0m\n");
		}
		
		printf(message.c_str());
		
		return failed ? 1 : 0;
	}
	
	int Case::run_all(int argc, char** argv) {
		printf("\x1b[1;36m%-20s%-40s%s\x1b[0m\n", "SUITE", "CASE", "RESULT");
		Case* c = first;
		if (c == NULL)
			printf("No tests.\n");
		while (c) {
			c->run();
			c = c->m_Next;
		}
		printf("%d passed, %d failed, %d pending (%d total)\n", passed_tests, failed_tests, pending_tests, passed_tests + failed_tests + pending_tests);
		return 0;
	}
	
	void Case::__set_suite_name(const char* name) {
		current_suite = name;
	}
}}
