#include "bench_local.hpp"
#include <cstdlib>

void benchmark_local::prepare( size_t num_threads )
{
total_pts_in_circle = 0;
total_pts_in_square = 0;
in_circle = new int32_t[ num_threads ];
in_square = new int32_t[ num_threads ];
}

void benchmark_local::prepare_thread( size_t thread_num, thread_control_block & tcb )
{
tcb.in_circle = &in_circle[thread_num];
tcb.in_square = &in_square[thread_num];
}

void benchmark_local::run_thread( thread_control_block & tcb )
{
volatile int32_t in_circle;
volatile int32_t in_square;

in_circle = 0;
in_square = 0;

for( int i = 0; i < tcb.num_points; ++i )
	{
	float x = (float)rand_r(&tcb.rand_seed)/(float)RAND_MAX;
	float y = (float)rand_r(&tcb.rand_seed)/(float)RAND_MAX;
	if( benchmark::pt_in_circle( x, y ) )
		{
		in_circle++;
        }
    else
        {
		in_square++;
        }
    }

*tcb.in_circle = in_circle;
*tcb.in_square = in_square;
}

void benchmark_local::finish_thread( thread_control_block & tcb )
{
total_pts_in_circle += *tcb.in_circle;
total_pts_in_square += *tcb.in_square;
}

void benchmark_local::finish( void )
{
delete[] in_circle;
delete[] in_square;
}
