noinst_LTLIBRARIES = libsnow-parser.la
libsnow_parser_la_SOURCES = \
	Driver.cpp \
	Driver.h \
	Parser.cpp \
	Parser.h \
	Scanner.cpp \
	Scanner.h
	
CLEANFILES = Scanner.cpp Parser.cpp Parser.h

Driver.cpp: Parser.h

Parser.cpp Parser.h: Parser.yy
	$(YACC) --defines=Parser.h -o Parser.cpp Parser.yy

Scanner.cpp: Scanner.ll Parser.h
	$(LEX) -o Scanner.cpp Scanner.ll

INCLUDES = -I@top_srcdir@/src


