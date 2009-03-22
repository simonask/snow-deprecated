AC_INIT(main.cpp)

AM_INIT_AUTOMAKE(snow,0.1)

AC_CONFIG_HEADERS(config.h)

AC_PROG_CC([ gcc-mp-4.4 , gcc-4.4 , gcc44 , gcc ])

AC_PROG_CXX([ g++-mp-4.4 , g++-4.4 , g++44 , g++ ])

AM_PROG_AS([ gcc-mp-4.4 , gcc-4.4 , gcc44 , gcc ])

AC_PROG_LEX([ flex ])

AC_PROG_YACC([ bison ])

CFLAGS="-O0 -g -m64 -DDEBUG -Wall"
__RELEASE_CFLAGS="-Os -m64 -DDEBUG -Wall -fomit-frame-pointer"
CXXFLAGS="$CFLAGS -std=c++0x"
CCASFLAGS="$CFLAGS"
LDFLAGS="-lpthread"

AC_PROG_LIBTOOL

AC_OUTPUT(Makefile src/Makefile src/lib/Makefile src/x86_64/Makefile test/Makefile src/parser/Makefile)
