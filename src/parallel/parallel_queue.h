#ifndef PARALLEL_QUEUE_H
#define PARALLEL_QUEUE_H

#include "base/types.h"
#include "atomics.h"
#include <stdbool.h>

//SPSC
typedef struct ParallelQueue ParallelQueue;
struct ParallelQueue {
	unsigned char  *buffer;
	u64 			elem_size;
	            	
	u64 			capacity;
	u64				mask;

	atomic_u64		producer_offset;
	atomic_u64		consumer_offset;
};

void 	init_parallel_queue		(ParallelQueue *q, u64 elem_size, u64 capacity);

bool 	produce_parallel_queue	(ParallelQueue *q, void *elem);
bool 	consume_parallel_queue	(ParallelQueue *q, void *out);

bool 	empty_parallel_queue	(ParallelQueue *q);

void 	destroy_parallel_queue	(ParallelQueue *q);

#endif