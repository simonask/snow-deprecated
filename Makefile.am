bin_PROGRAMS = snot
snot_SOURCES = main.cpp asmtest.s asmtest.h
INCLUDES = -I@top_srcdir@/src
SUBDIRS = src
LDADD = src/libsnot.a src/lib/libsnot-lib.a src/parser/libsnot-parser.a src/x86_64/libsnot-x86_64.a

AUTOMAKE_OPTIONS = foreign
