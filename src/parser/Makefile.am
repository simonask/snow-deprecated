noinst_LIBRARIES = libsnow-parser.a
libsnow_parser_a_SOURCES = \
	Driver.h \
	Driver.cpp \
	Parser.h \
	Scanner.h \
	Scanner.cpp \
	Parser.cpp
 
INCLUDES = -I@top_srcdir@/src

CLEANFILES = Scanner.cpp Parser.cpp Parser.h

Scanner.cpp: Scanner.ll
	$(LEX) -o Scanner.cpp Scanner.ll

Parser.cpp Parser.h: Parser.yy
	$(YACC) --defines=Parser.h -o Parser.cpp Parser.yy
