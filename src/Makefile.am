noinst_LIBRARIES = libsnot.a
libsnot_a_SOURCES = Assembler.cpp Assembler.h ASTNode.cpp ASTNode.h Basic.h Codegen.h CompiledCode.cpp CompiledCode.h Label.h Linker.cpp Linker.h Operand.h SymbolTable.cpp SymbolTable.h Util.cpp Util.h Scope.h Iterate.h
INCLUDES = -I@top_srcdir@/src
SUBDIRS = lib parser x86_64 ast
libsnot_a_LIBADD = lib/libsnot-lib.a x86_64/libsnot-x86_64.a ast/libsnot-ast.a