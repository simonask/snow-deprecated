bin_PROGRAMS = snow
snow_SOURCES = main.cpp
INCLUDES = -I@top_srcdir@/src
SUBDIRS = src test src/parser
snow_LDADD = src/libsnow.a src/lib/libsnow-lib.a src/x86_64/libsnow-x86_64.a src/parser/libsnow-parser.a

AUTOMAKE_OPTIONS = foreign

test: all
	test/runner
