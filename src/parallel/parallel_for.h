#ifndef PARALLEL_FOR_H
#define PARALLEL_FOR_H

#include "thread_pool.h"

void parallel_for(
	ThreadPool *pool,
	int start,
	int end,
	int grain_size,
	void (*func)(void *data, int start, int end),
	void *data
);

#endif