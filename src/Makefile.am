noinst_LIBRARIES = libsnow.a
libsnow_a_SOURCES = Assembler.cpp Assembler.h ASTNode.h Basic.h Codegen.cpp Codegen.h CompiledCode.cpp CompiledCode.h Label.h Linker.cpp Linker.h Operand.h SymbolTable.cpp SymbolTable.h Util.cpp Util.h Scope.h Iterate.h Realizer.cpp Realizer.h StackFrame.cpp StackFrame.h
INCLUDES = -I@top_srcdir@/src
SUBDIRS = lib parser x86_64
libsnow_a_LIBADD = lib/libsnow-lib.a x86_64/libsnow-x86_64.a