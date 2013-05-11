CXX=g++ 
CXXFLAGS+=-Wall -Wextra -g -fopenmp

all: seq par

par: main.o poly_par.o parser.o
	$(CXX) $(CXXFLAGS) main.o poly_par.o parser.o -o par 

seq: main.o poly_seq.o parser.o
	$(CXX) $(CXXFLAGS) main.o poly_seq.o parser.o -o seq

poly_par.o: poly_par.cpp polynomial.h
	$(CXX) $(CXXFLAGS) -c poly_par.cpp

poly_seq.o: poly_seq.cpp polynomial.h
	$(CXX) $(CXXFLAGS) -c poly_seq.cpp

parser.o: parser.cpp parser.h
	$(CXX) $(CXXFLAGS) -c parser.cpp

main.o: main.cpp polynomial.h parser.h
	$(CXX) $(CXXFLAGS) -c main.cpp

clean: 
	rm *.o
