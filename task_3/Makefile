CXX = g++
CXXFLAGS = -std=c++17
EXECUTABLE = run
LIBS = -lboost_system -lboost_thread -lstdc++fs

all: run

run: counter.o main.o 
	$(CXX) $(CXXFLAGS) -o run main.o counter.o $(LIBS)

main.o: main.cpp counter.h
	$(CXX) $(CXXFLAGS) -o main.o -c main.cpp $(LIBS)

counter.o: counter.h counter.cpp 
	$(CXX) $(CXXFLAGS) -o counter.o -c counter.cpp $(LIBS)

clean:
	clean:
	#rm -rf *.o 
	#rm -rf run
	$(RM) run
	$(RM) *.o

