all:
	g++-mp-4.4 -std=c++0x -m64 -g -o snot -Isrc *.cpp src/*.cpp src/x86_64/*.cpp src/lib/*.cpp asmtest.s