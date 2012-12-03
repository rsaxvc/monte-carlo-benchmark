#define MODE_SEMAPHORE
#define MODE_ATOMIC_OPERATORS
#define MODE_THREAD_STORAGE
#define MODE_LOCAL_THREAD_STORAGE

CXX=g++
CXXFLAGS=-march=native -O4 -c
CC=$(CXX)
LDFLAGS=-lpthread
BUILDS=mc_semaphore mc_atomics mc_thread_storage mc_local_thread_storage

all:$(BUILDS)

main.o: main.cpp

mc_semaphore.o: main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ -D MODE_SEMAPHORE

mc_atomics.o: main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ -D MODE_ATOMIC_OPERATORS

mc_thread_storage.o: main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ -D MODE_THREAD_STORAGE

mc_local_thread_storage.o: main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ -D MODE_LOCAL_THREAD_STORAGE

clean:
	rm -f *.o $(BUILDS)
