noinst_PROGRAMS = runner
runner_SOURCES = runner.cpp test.cpp \
	ArrayTest.cpp \
	CodegenTest.cpp \
	FloatTest.cpp \
	IntegerTest.cpp \
	NumericTest.cpp \
	ParserTest.cpp \
	StringTest.cpp \
	ExceptionTest.cpp \
	RegExTest.cpp \
	RBTreeTest.cpp

INCLUDES = -I@top_srcdir@/src
runner_LDADD = ../src/.libs/libsnow.a ../src/3rdparty/onig/.libs/libonig.a
AM_CXXFLAGS = -Wno-unused-label

AUTOMAKE_OPTIONS = foreign
