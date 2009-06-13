SUBDIRS = base codegen gc parser runtime 3rdparty/onig
lib_LTLIBRARIES = libsnow.la
libsnow_la_SOURCES = 
libsnow_la_LIBADD = base/libsnow-base.la codegen/libsnow-codegen.la gc/libsnow-gc.la  parser/libsnow-parser.la runtime/libsnow-runtime.la 3rdparty/onig/libonig.la
#libsnow_la_DEPENDENCIES = $(libsnow_la_LDADD)

INCLUDES = -I@top_srcdir@/src -I@top_srcdir@/src/base

