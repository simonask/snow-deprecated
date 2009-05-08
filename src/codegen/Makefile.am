SUBDIRS = x86_64
noinst_LTLIBRARIES = libsnow-codegen.la
libsnow_codegen_la_SOURCES = \
	ASTNode.h \
	Assembler.cpp \
	Assembler.h \
	Codegen.cpp \
	Codegen.h \
	CompiledCode.cpp \
	CompiledCode.h \
	Label.h \
	Linker.cpp \
	Linker.h \
	Operand.h \
	XMLCodegen.cpp \
	XMLCodegen.h
libsnow_codegen_la_LIBADD = x86_64/libsnow-codegen-x86_64.la

INCLUDES = -I@top_srcdir@/src -I@top_srcdir@/src/codegen

