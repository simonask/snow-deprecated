AC_INIT(main.cpp)

AM_INIT_AUTOMAKE(snow,0.1)

AC_CONFIG_HEADERS(config.h)

AC_PROG_CC([ gcc-mp-4.4 , gcc-4.4 , gcc44 , gcc ])

AC_PROG_CXX([ g++-mp-4.4 , g++-4.4 , g++44 , g++ ])

AM_PROG_AS([ gcc-mp-4.4 , gcc-4.4 , gcc44 , gcc ])

CFLAGS="-m64 -g -O0 -DDEBUG -Wall -Werror"
CXXFLAGS="-std=c++0x $CFLAGS"
CCASFLAGS="$CFLAGS"
LDFLAGS="-lpthread"

AC_PROG_LIBTOOL

AC_OUTPUT(Makefile src/Makefile src/lib/Makefile src/x86_64/Makefile)
