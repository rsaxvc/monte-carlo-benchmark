#include <cstdlib>
#include <cmath>
#include <iostream>

#include <pthread.h>

#include <time.h>
#include <sys/time.h>

struct thread_control_block
	{
	pthread_t id;
	int num_points;
	unsigned int rand_seed;
	#if defined( MODE_THREAD_STORAGE ) || defined( MODE_LOCAL_THREAD_STORAGE )
	size_t in_circle;
	size_t in_square;
	#endif
	};

#ifdef MODE_SEMAPHORE
static pthread_mutex_t lock;
#endif

#if defined( MODE_SEMAPHORE ) || defined( MODE_ATOMIC_OPERATORS )
static size_t in_circle;
static size_t in_square;
#endif

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

#if defined( MODE_SEMAPHORE )
	static __inline void record_circle()
		{
		pthread_mutex_lock( &lock );
		in_circle++;
		pthread_mutex_unlock( &lock );
		}
	static __inline void record_square()
		{
		pthread_mutex_lock( &lock );
		in_square++;
		pthread_mutex_unlock( &lock );
		}
#elif defined( MODE_ATOMIC_OPERATORS )
	#define record_circle() __sync_fetch_and_add( &in_circle, 1 )
	#define record_square() __sync_fetch_and_add( &in_square, 1 )
#elif defined( MODE_THREAD_STORAGE )
	#define record_circle() tcb->in_circle++
	#define record_square() tcb->in_square++
#elif defined( MODE_LOCAL_THREAD_STORAGE )
	#define record_circle() in_circle++
	#define record_square() in_square++
#else
	#error
#endif

static void * montecarlo_pi( void * cntptr )
{
#ifdef MODE_LOCAL_THREAD_STORAGE
size_t in_circle=0;
size_t in_square=0;
#endif

thread_control_block * tcb = (thread_control_block*)cntptr;
#ifdef MODE_THREAD_STORAGE
tcb->in_circle = 0;
tcb->in_square = 0;
#endif
for( int i = 0; i < tcb->num_points; ++i )
	{
	float x = (float)rand_r(&tcb->rand_seed)/(float)RAND_MAX;
	float y = (float)rand_r(&tcb->rand_seed)/(float)RAND_MAX;
	if( pt_in_circle( x, y ) )
		{
		record_circle();
		}
	else
		{
		record_square();
		}
	}

#ifdef MODE_LOCAL_THREAD_STORAGE
tcb->in_circle = in_circle;
tcb->in_square = in_square;
#endif
}
int main( int numArgs, const char * args[] )
{
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

#ifdef MODE_SEMAPHORE
pthread_mutex_init( &lock, NULL );
#endif

#ifdef DEBUG
std::cout<<"Approximating the value of PI using "<<count<<" pseudorandomly placed points and "<<nthreads<<" threads"<<std::endl;
#endif

tcbs = (thread_control_block*)malloc( sizeof( thread_control_block ) * nthreads );
approx_points_per_thread = count / nthreads;
for( size_t i = 0; i < nthreads; ++i )
	{
	tcbs[i].num_points=approx_points_per_thread;
	count -= approx_points_per_thread;
	tcbs[i].rand_seed = i;
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

#if defined( MODE_THREAD_STORAGE ) || defined( MODE_LOCAL_THREAD_STORAGE )
size_t in_circle = 0;
size_t in_square = 0;
#endif
for( size_t i = 0; i < nthreads; ++i )
	{
	pthread_join( tcbs[i].id, NULL );
	#if defined( MODE_THREAD_STORAGE ) || defined( MODE_LOCAL_THREAD_STORAGE )
	in_circle += tcbs[i].in_circle;
	in_square += tcbs[i].in_square;
	#endif
	}
long double work_done_time = get_raw_time();
free( tcbs );

#ifdef MODE_SEMAPHORE
pthread_mutex_destroy( &lock );
#endif

#ifdef DEBUG
std::cout<<"in circle:"<<in_circle<<" points"<<std::endl;
std::cout<<"in square:"<<in_square<<" points"<<std::endl;
std::cout<<"pi approx:"<<4*((float)in_circle)/(in_circle+in_square)<<std::endl;
std::cout<<"Took "<<work_done_time - start_work_time <<" seconds to process"<<std::endl;
#else
std::cout<<work_done_time - start_work_time;
#endif
}
