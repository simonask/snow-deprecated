bin_PROGRAMS = snow
snow_SOURCES = main.cpp
INCLUDES = -I@top_srcdir@/src
SUBDIRS = src test
snow_LDADD = src/libsnow.a -lreadline

AUTOMAKE_OPTIONS = foreign

test: all
	test/runner
