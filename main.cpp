#include <cstdlib>
#include <cmath>
#include <iostream>

#include <pthread.h>

#include <time.h>
#include <sys/time.h>

static long double get_raw_time()
{
timeval tp;
gettimeofday( &tp, NULL);
return tp.tv_sec + (long double)tp.tv_usec / 1000000;
}


static __inline bool pt_in_circle( float x, float y )
{
return ( x*x + y*y ) < 1.0f;
}

struct thread_control_block
	{
	pthread_t id;
	int num_points;
	unsigned int rand_seed;
	size_t in_circle;
	size_t in_square;
	size_t padding[1024];
	};

static void * montecarlo_pi( void * cntptr )
{
thread_control_block * tcb = (thread_control_block*)cntptr;
for( int i = 0; i < tcb->num_points; ++i )
	{
	float x = (float)rand_r(&tcb->rand_seed)/(float)RAND_MAX;
	float y = (float)rand_r(&tcb->rand_seed)/(float)RAND_MAX;
	if( pt_in_circle( x, y ) )
		{
		tcb->in_circle++;
		}
	else
		{
		tcb->in_square++;
		}
	}
}

int main( int numArgs, const char * args[] )
{
size_t in_circle;
size_t in_square;
size_t count;
size_t nthreads;
int approx_points_per_thread;
thread_control_block * tcbs;

if( numArgs != 3 )
	{
	std::cout<<"Usage: "<<args[0]<<" numPoints numThreads"<<std::endl;
	exit(2);
	}

count = atoi(args[1]);
nthreads = atoi(args[2]);

std::cout<<"Approximating the value of PI using "<<count<<" pseudorandomly placed points and "<<nthreads<<" threads"<<std::endl;

tcbs = (thread_control_block*)malloc( sizeof( thread_control_block ) * nthreads );
approx_points_per_thread = count / nthreads;
for( size_t i = 0; i < nthreads; ++i )
	{
	tcbs[i].num_points=approx_points_per_thread;
	count -= approx_points_per_thread;
	tcbs[i].rand_seed = i;
	tcbs[i].in_circle = 0;
	tcbs[i].in_square = 0;
	}
while( count > 0 )
	{
	tcbs[ count % nthreads ].num_points ++;
	count--;
	}

long double start_work_time = get_raw_time();

for( size_t i = 0; i < nthreads; ++i )
	{
	pthread_create( &tcbs[i].id, NULL, montecarlo_pi, &tcbs[i] );
	}

in_circle = 0;
in_square = 0;
for( size_t i = 0; i < nthreads; ++i )
	{
	pthread_join( tcbs[i].id, NULL );
	in_circle += tcbs[i].in_circle;
	in_square += tcbs[i].in_square;
	}
long double work_done_time = get_raw_time();
free( tcbs );


std::cout<<"in circle:"<<in_circle<<" points"<<std::endl;
std::cout<<"in square:"<<in_square<<" points"<<std::endl;
std::cout<<"pi approx:"<<4*((float)in_circle)/(in_circle+in_square)<<std::endl;
std::cout<<"Took "<<work_done_time - start_work_time <<" seconds to process"<<std::endl;
}
