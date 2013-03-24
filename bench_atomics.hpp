#ifndef BENCH_ATOMICS_HPP
#define BENCH_ATOMICS_HPP

#include "bench.hpp"

class benchmark_atomics:public benchmark
    {
	int32_t in_circle;
	int32_t in_square;
	virtual void prepare( size_t num_threads ){in_circle=0;in_square=0;};
	virtual void prepare_thread( size_t thread_num, thread_control_block & tcb )
		{
		tcb.in_circle = &in_circle;
		tcb.in_square = &in_square;
		};
    virtual void run_thread( thread_control_block & );
	virtual void finish_thread( thread_control_block & tcb ){};
	virtual void finish( void );
    };

#endif
