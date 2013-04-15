CXX=g++ 
CXXFLAGS+=-Wall -Wextra -O2

all: seq

seq: main.o polynomial.o
	$(CXX) $(CXXFLAGS) main.o polynomial.o -o seq

polynomial.o: polynomial.cpp polynomial.h
	$(CXX) $(CXXFLAGS) -c polynomial.cpp

main.o: main.cpp polynomial.h
	$(CXX) $(CXXFLAGS) -c main.cpp
