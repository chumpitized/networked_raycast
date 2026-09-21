#ifndef JOB_COUNTER_H
#define JOB_COUNTER_H

typedef struct JobCounter JobCounter;
struct JobCounter {
	int count;

	void *cv;
	void *mutex;
};

void job_counter_init		(JobCounter *counter);
void job_counter_begin		(JobCounter *counter, int count);
void job_counter_finish_one	(JobCounter *counter);
void job_counter_wait		(JobCounter *counter);
void job_counter_destroy	(JobCounter *counter);

typedef struct CountedJob CountedJob;
struct CountedJob {
	void (*func)(void *);
	void *data;

	JobCounter *counter;
};

void counted_job_run(void *func);

#endif