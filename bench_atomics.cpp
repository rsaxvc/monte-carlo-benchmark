#include "bench_atomics.hpp"
#include <cstdlib>

void benchmark_atomics::run_thread( thread_control_block & tcb )
{
for( int i = 0; i < tcb.num_points; ++i )
	{
	float x = (float)rand_r(&tcb.rand_seed)/(float)RAND_MAX;
	float y = (float)rand_r(&tcb.rand_seed)/(float)RAND_MAX;
	if( benchmark::pt_in_circle( x, y ) )
		{
		__sync_fetch_and_add( &in_circle, 1 );
        }
    else
        {
		__sync_fetch_and_add( &in_square, 1 );
        }
    }
}

void benchmark_atomics::finish( void )
{
total_pts_in_circle = in_circle;
total_pts_in_square = in_square;
}
