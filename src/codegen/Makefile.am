if ARCH_x86_64
SUBDIRS_x86_64 = x86_64
LIBADD_x86_64 = x86_64/libsnow-codegen-x86_64.la
endif

if ARCH_x86_32
SUBDIRS_x86_32 = x86_32
LIBADD_x86_32 = x86_32/libsnow-codegen-x86_32.la
endif

SUBDIRS = $(SUBDIRS_x86_32) $(SUBDIRS_x86_64)

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
	Operand.h

libsnow_codegen_la_LIBADD = $(LIBADD_x86_32) $(LIBADD_x86_64)

INCLUDES = -I@top_srcdir@/src -I@top_srcdir@/src/codegen

