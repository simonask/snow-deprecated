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
LDADD = ../src/libsnow.a ../src/lib/libsnow-lib.a ../src/x86_64/libsnow-x86_64.a ../src/parser/libsnow-parser.a
AM_CXXFLAGS = -Wno-unused-label

AUTOMAKE_OPTIONS = foreign
