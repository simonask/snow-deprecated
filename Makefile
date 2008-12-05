all:
	g++ -m64 -g -o codegen -Isrc *.cpp asmtest.s src/*.cpp