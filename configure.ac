AC_INIT(main.cpp)

AM_INIT_AUTOMAKE(snow,0.1)

AC_CONFIG_HEADERS(config.h)

AC_PROG_CC([ gcc-mp-4.4 , gcc-4.4 , gcc44 , gcc ])

AC_PROG_CXX([ g++-mp-4.4 , g++-4.4 , g++44 , g++ ])

AM_PROG_AS([ gcc-mp-4.4 , gcc-4.4 , gcc44 , gcc ])

AC_PROG_LEX([ flex ])

AC_PROG_YACC([ bison ])

LT_INIT([dlopen])

__DEBUG_CFLAGS="-O0 -pg -g -m64 -DDEBUG -Wall"
__RELEASE_CFLAGS="-Os -m64 -pipe -fomit-frame-pointer"
CFLAGS=$__DEBUG_CFLAGS
CXXFLAGS="$CFLAGS -std=c++0x"
CCASFLAGS="$CFLAGS"
LDFLAGS="-lpthread"

AC_PROG_LIBTOOL

sLIBS=$LIBS
LIBS=""
AC_SEARCH_LIBS(readline, readline, [], [])
AC_CHECK_FUNCS(readline, [], [])
READLINE_LIBS=$LIBS
LIBS=$sLIBS
AC_SUBST(READLINE_LIBS)

AC_OUTPUT(Makefile src/Makefile test/Makefile src/parser/Makefile src/base/Makefile src/codegen/Makefile src/codegen/x86_64/Makefile src/gc/Makefile src/runtime/Makefile)
