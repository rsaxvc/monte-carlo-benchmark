#ifndef BENCH_SHARED_HPP
#define BENCH_SHARED_HPP

#include "bench.hpp"

class benchmark_shared:public benchmark
	{
	int * in_circle;
	int * in_square;

	virtual void prepare( size_t num_threads );
	virtual void prepare_thread(size_t, thread_control_block&);
	virtual void run_thread( thread_control_block & );
	virtual void finish_thread( thread_control_block&);
	virtual void finish( void );
	};

#endif
