lib_LIBRARIES = libsnow.a
libsnow_a_SOURCES = \
	MemoryManager.cpp \
	MemoryManager.h \
	GarbageAllocator.h \
	GarbageAllocator.cpp \
	Garbage.h \
	Garbage.cpp \
	ExecutableAllocator.h \
	ExecutableAllocator.cpp \
	Assembler.cpp \
 	Assembler.h \
 	ASTNode.h \
 	Basic.h \
 	Codegen.cpp \
 	Codegen.h \
 	CompiledCode.cpp \
 	CompiledCode.h \
 	Label.h \
 	Linker.cpp \
 	Linker.h \
 	Operand.h \
 	SymbolTable.cpp \
 	SymbolTable.h \
 	Util.cpp \
 	Util.h \
 	Scope.h \
 	Iterate.h \
 	RefPtr.h \
 	TempAllocator.h \
 	SuperFastHash.h \
 	SuperFastHash.cpp \
 	Internal.h \
 	LocalMap.h \
 	LocalMap.cpp

INCLUDES = -I@top_srcdir@/src
SUBDIRS = lib x86_64
libsnow_a_LIBADD = lib/libsnow-lib.a x86_64/libsnow-x86_64.a