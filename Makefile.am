bin_PROGRAMS = snow
snow_SOURCES = main.cpp asmtest.s asmtest.h
INCLUDES = -I@top_srcdir@/src
SUBDIRS = src test src/parser
LDADD = src/libsnow.a src/lib/libsnow-lib.a src/x86_64/libsnow-x86_64.a

AUTOMAKE_OPTIONS = foreign
