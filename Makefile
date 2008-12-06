all:
	g++ -m64 -g -o snot -Isrc *.cpp src/*.cpp src/x86_64/*.cpp