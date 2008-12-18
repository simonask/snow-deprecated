#!/bin/sh
if [ -e Makefile ]; then
	make clean;
fi

./autogen.sh

make
