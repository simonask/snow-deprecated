noinst_PROGRAMS = runner
runner_SOURCES = runner.cpp test.cpp \
	ArrayTest.cpp \
	CodegenTest.cpp \
	FloatTest.cpp \
	IntegerTest.cpp \
	ParserTest.cpp \
	ParserExpressionTest.cpp \
	StringTest.cpp

INCLUDES = -I@top_srcdir@/src
runner_LDADD = -L../src -lsnow
AM_CXXFLAGS = -Wno-unused-label

AUTOMAKE_OPTIONS = foreign
