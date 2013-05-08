CXX=g++ 
CXXFLAGS+=-Wall -Wextra -g

all: seq

seq: main.o polynomial.o parser.o
	$(CXX) $(CXXFLAGS) main.o polynomial.o parser.o -o seq

polynomial.o: polynomial.cpp polynomial.h
	$(CXX) $(CXXFLAGS) -c polynomial.cpp

parser.o: parser.cpp parser.h
	$(CXX) $(CXXFLAGS) -c parser.cpp

main.o: main.cpp polynomial.h parser.h
	$(CXX) $(CXXFLAGS) -c main.cpp
