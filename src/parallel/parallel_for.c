#include "parallel_for.h"

#include "job_counter.h"

#include <assert.h>

#define MAX_PARALLELISM 64

typedef struct ParallelForCountedJob ParallelForCountedJob;
struct ParallelForCountedJob {
    void (*func)(void *data, int start, int end);
    void *data;

    int start;
    int end;

	JobCounter *counter;
};

static void parallel_for_job_counted_func(void *parallel_for_counted_job) {
    ParallelForCountedJob *job = (ParallelForCountedJob *)parallel_for_counted_job;

    job->func(job->data, job->start, job->end);

	job_counter_finish_one(job->counter);
}

void parallel_for(
	ThreadPool *pool,
	int start,
	int end,
	int grain_size,
	void (*func)(void *data, int start, int end),
	void *data
) {
	assert(pool && "Invalid thread pool!");
	assert(func && "Invalid function pointer!");
	assert(grain_size > 0 && "Grain size is 0!");

	int total_item_count = end - start;

	if (total_item_count <= 0) {
		return;
	}

	int desired_job_count 	= (total_item_count + grain_size - 1) / grain_size;
	int max_job_count 		= pool->thread_count > MAX_PARALLELISM ? MAX_PARALLELISM : pool->thread_count;
	int job_count			= desired_job_count > max_job_count ? max_job_count : desired_job_count;

	if (job_count <= 1) {
		func(data, start, end);
		return;
	}

	int worker_count = job_count;

	ParallelForCountedJob jobs[worker_count];

	JobCounter counter;
	job_counter_init(&counter);
	job_counter_begin(&counter, worker_count);

	int remaining_items;
	int remaining_jobs;
	int chunk_size;
	int current = start;

	for (int i = 0; i < worker_count; ++i) {
		remaining_items = end - current;
		remaining_jobs 	= job_count - i;		
		chunk_size 		= (remaining_items + remaining_jobs - 1) / remaining_jobs;

		jobs[i].func 	= func;
		jobs[i].data 	= data;
		jobs[i].start 	= current;
		jobs[i].end		= current + chunk_size;
		jobs[i].counter	= &counter;

		current += chunk_size;

		if (!enqueue_thread_pool(pool, parallel_for_job_counted_func, &jobs[i])) {
			parallel_for_job_counted_func(&jobs[i]);
		}
	}

	job_counter_wait(&counter);	
	job_counter_destroy(&counter);
}