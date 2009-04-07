
CLEANFILES = Scanner.cpp Parser.cpp Parser.h

Scanner.cpp: Scanner.ll Parser.h
	$(LEX) -o Scanner.cpp Scanner.ll

Parser.cpp Parser.h: Parser.yy
	$(YACC) --defines=Parser.h -o Parser.cpp Parser.yy

all: Parser.cpp Parser.h Scanner.cpp



