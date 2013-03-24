CXX=g++
CXXFLAGS=-march=native -O3 -g
CC=$(CXX)
LDFLAGS=-lpthread
BUILDS=bench
NUM_POINTS=1000000
SOURCE=*.cpp

all:$(BUILDS)

bench: $(SOURCE)
	g++ $(SOURCE) -o bench $(LDFLAGS) $(CXXFLAGS)

benchmarks: $(BENCHMARK_FILES)

main.o: main.cpp

clean:
	rm -f *.o $(BUILDS)

