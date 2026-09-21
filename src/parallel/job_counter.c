#include "job_counter.h"

#include "os/mutex.h"
#include "os/cond_variable.h"
#include "assert.h"

void job_counter_init(JobCounter *counter) {
	counter->count 	= 0;
	counter->mutex 	= create_mutex();
	counter->cv 	= create_cv();
}

void job_counter_begin(JobCounter *counter, int count) {
	assert(count > 0 && "Worker count is zero!");

	lock_mutex(counter->mutex);
	counter->count = count;
	unlock_mutex(counter->mutex);
}

void job_counter_finish_one(JobCounter *counter) {
	lock_mutex(counter->mutex);

	assert(counter->count > 0 && "Job counter is already 0!");;

	counter->count -= 1;

	if (counter->count <= 0) {
		cv_signal_all(counter->cv);
	}

	unlock_mutex(counter->mutex);
}

void job_counter_wait(JobCounter *counter) {
	lock_mutex(counter->mutex);

    while (counter->count > 0) {
        cv_wait(counter->cv, counter->mutex);
    }

    unlock_mutex(counter->mutex);
}

void job_counter_destroy(JobCounter *counter) {
	destroy_mutex(counter->mutex);
	cv_destroy(counter->cv);

	counter->count 	= 0;
	counter->mutex 	= 0;
	counter->cv 	= 0;
}

void counted_job_run(void *data) {
	CountedJob *job = (CountedJob *)data;

	job->func(job->data);

	job_counter_finish_one(job->counter);
}