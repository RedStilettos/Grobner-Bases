CXX=g++
CXXFLAGS+=-Wall -Wextra -O2

seq: seq.o
	$(CXX) $(CXXFLAGS) -o seq.o 

Main.o : main.cpp Polynomial.h
	$(CXX) $(CXXFLAGS) main.cpp
