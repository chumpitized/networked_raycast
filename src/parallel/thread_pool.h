#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include "os/thread.h"
#include "base/fixed_queue.h"

typedef struct ThreadPool ThreadPool;
struct ThreadPool {
	Thread 	   	   *threads;
	int 			thread_count;

	FixedQueue	   *queue;
	void 	  	   *mutex;
	void 	  	   *cv;
	volatile long 	stop;
};

typedef struct Task Task;
struct Task {
	void (*func)(void *data);
	void *data;
};

void worker_thread(void *arg);

void init_thread_pool		(ThreadPool *p, int thread_count, int work_queue_size);
bool enqueue_thread_pool	(ThreadPool *p, void (*func)(void *data), void *data);
void stop_thread_pool		(ThreadPool *p);
void destroy_thread_pool	(ThreadPool *p);

#endif