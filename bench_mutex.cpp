#include "bench_mutex.hpp"
#include <cstdlib>

void benchmark_mutex::prepare( size_t num_threads )
{
total_pts_in_circle = 0;
total_pts_in_square = 0;
pthread_mutex_init( &circle_lock, NULL );
pthread_mutex_init( &square_lock, NULL );
}

void benchmark_mutex::run_thread( thread_control_block & tcb )
{
for( int i = 0; i < tcb.num_points; ++i )
	{
	float x = (float)rand_r(&tcb.rand_seed)/(float)RAND_MAX;
	float y = (float)rand_r(&tcb.rand_seed)/(float)RAND_MAX;
	if( benchmark::pt_in_circle( x, y ) )
		{
        pthread_mutex_lock( &circle_lock );
        in_circle++;
        pthread_mutex_unlock( &circle_lock );
        }
    else
        {
        pthread_mutex_lock( &square_lock );
        in_square++;
        pthread_mutex_unlock( &square_lock );
        }
    }
}

void benchmark_mutex::finish()
{
total_pts_in_circle = in_circle;
total_pts_in_square = in_square;
pthread_mutex_destroy(&circle_lock);
pthread_mutex_destroy(&square_lock);
}
