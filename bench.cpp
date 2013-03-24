#include "bench.hpp"

float benchmark::result( void )
{
return 4.0 * (double)total_pts_in_circle / ( (double)total_pts_in_square + (double)total_pts_in_circle );
}

void * benchmark::threadrunner( void * vptr )
{
thread_control_block * tcb = (benchmark::thread_control_block*)vptr;
benchmark * b = tcb->bptr;
b->run_thread( *tcb );
}

void benchmark::run( size_t num_points, size_t num_threads )
{
int approx_points_per_thread;
thread_control_block * tcbs;

prepare( num_threads );

tcbs = new thread_control_block[num_threads];
approx_points_per_thread = num_points / num_threads;
for( size_t i = 0; i < num_threads; ++i )
    {
    tcbs[i].num_points=approx_points_per_thread;
    num_points -= approx_points_per_thread;

    tcbs[i].rand_seed = i;

	tcbs[i].bptr = this;
	prepare_thread( i, tcbs[i] );
    }
while( num_points > 0 )
    {
    tcbs[ num_points % num_threads ].num_points ++;
    num_points--;
    }

for( size_t i = 0; i < num_threads; ++i )
    {
    pthread_create( &tcbs[i].id, NULL, threadrunner, &tcbs[i] );
    }

for( size_t i = 0; i < num_threads; ++i )
    {
    pthread_join( tcbs[i].id, NULL );
	finish_thread( tcbs[i] );
    }

delete[]( tcbs );

finish();
}
