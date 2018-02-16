all: spfind

spfind: main.cpp Sequence.hpp Sequence.ipp State.hpp State.cpp
	g++ -std=c++14 -O3 -o spfind main.cpp State.cpp
