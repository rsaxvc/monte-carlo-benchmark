CXX=g++
CXXFLAGS=-march=native -O4 -c
CC=$(CXX)
LDFLAGS=-lpthread
BUILDS=mc_semaphore mc_atomics mc_thread_storage mc_local_thread_storage
NUM_POINTS=1000000
BENCHMARK_FILES=\
	mc_atomics_$(NUM_POINTS).csv\
	mc_semaphore_$(NUM_POINTS).csv\
	mc_thread_storage_$(NUM_POINTS).csv\
	mc_local_thread_storage_$(NUM_POINTS).csv

all:$(BUILDS)

graph.png: benchmarks
	./plot_results.sh

benchmarks: $(BENCHMARK_FILES)

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

mc_atomics_$(NUM_POINTS).csv: mc_atomics
	./run_thread_benchmark.sh atomics $(NUM_POINTS)

mc_semaphore_$(NUM_POINTS).csv: mc_semaphore
	./run_thread_benchmark.sh semaphore $(NUM_POINTS)

mc_thread_storage_$(NUM_POINTS).csv: mc_thread_storage
	./run_thread_benchmark.sh thread_storage $(NUM_POINTS)

mc_local_thread_storage_$(NUM_POINTS).csv: mc_local_thread_storage
	./run_thread_benchmark.sh local_thread_storage $(NUM_POINTS)
