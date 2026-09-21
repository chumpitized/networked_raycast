#include "thread_pool.h"

#include <windows.h>
#include <stdio.h>

#include "base/fixed_queue.h"

#include "os/mutex.h"
#include "os/cond_variable.h"

void worker_thread(void *arg) {
	ThreadPool *p = (ThreadPool *)arg;

	FixedQueue *queue 	= p->queue;
	void *mutex 		= p->mutex;
	void *cv 			= p->cv;

	for(;;) {
		lock_mutex(mutex);

		while (!p->stop && empty_fixed_queue(queue)) {
			SleepConditionVariableCS(cv, mutex, INFINITE);
		}

		if (p->stop && empty_fixed_queue(queue)) {
			unlock_mutex(mutex);
			return;
		}

		Task task = *(Task *)pop_fixed_queue(queue);
			
		unlock_mutex(mutex);

		task.func(task.data);			
	}
}

void init_thread_pool(ThreadPool *p, int thread_count, int work_queue_size) {
	/*could use different allocator...*/
	FixedQueue *queue = (FixedQueue *)malloc(sizeof(FixedQueue));
	init_fixed_queue(queue, sizeof(Task), work_queue_size);

	p->queue 		= queue;
	p->mutex 		= create_mutex();
	p->cv 			= create_cv();
	p->threads 		= (Thread *)malloc(sizeof(Thread) * thread_count);
	p->thread_count = thread_count;
	p->stop 		= false;

	int i;
	for (i = 0; i < thread_count; ++i) {
		p->threads[i] = create_thread(worker_thread, p);
	}
}

bool enqueue_thread_pool(ThreadPool *p, void (*func)(void *data), void *data) {
	Task task = {
		.func = func,
		.data = data
	};	
	
	lock_mutex(p->mutex);

	if (p->stop) {
		unlock_mutex(p->mutex);
		return false;
	}
	
	bool pushed = push_fixed_queue(p->queue, &task);

	unlock_mutex(p->mutex);

	if (pushed) {
		cv_signal_one(p->cv);
	}

	return pushed;
}

void stop_thread_pool(ThreadPool *p) {
	lock_mutex(p->mutex);
	p->stop = true;
	unlock_mutex(p->mutex);

	cv_signal_all(p->cv);

	WaitForMultipleObjects(p->thread_count, p->threads, TRUE, INFINITE);

	int i;
	for (i = 0; i < p->thread_count; ++i) {
		CloseHandle(p->threads[i]);
	}
	free(p->threads);
	p->threads = NULL;
}

void destroy_thread_pool(ThreadPool *p) {
	if (!p->stop) {
		stop_thread_pool(p);
	}

	destroy_fixed_queue(p->queue);
	free(p->queue);

	destroy_mutex(p->mutex);

	cv_destroy(p->cv);

	p->queue 	= NULL;
	p->mutex 	= NULL;
	p->cv 		= NULL;
}