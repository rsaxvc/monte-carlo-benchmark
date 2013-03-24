#ifndef BENCH_LOCAL_HPP
#define BENCH_LOCAL_HPP

#include "bench.hpp"

class benchmark_local:public benchmark
	{
	int32_t * in_circle;
	int32_t * in_square;

	virtual void prepare( size_t num_threads );
	virtual void prepare_thread( size_t thread_num, thread_control_block & tcb );
	virtual void run_thread( thread_control_block & );
	virtual void finish_thread( thread_control_block & tcb );
	virtual void finish(void);
	};

#endif
