#include <windows.h>
#include <assert.h>

#include "parallel_queue.h"

//update with better queue now...

bool is_power_of_two(u64 x) {
	return (x & (x - 1)) == 0;
}

void init_parallel_queue(ParallelQueue *q, u64 elem_size, u64 capacity) {
	assert(is_power_of_two(capacity));

	q->buffer 			= (unsigned char *)VirtualAlloc(NULL, capacity * elem_size, MEM_COMMIT, PAGE_READWRITE);
	q->elem_size 		= elem_size;

	q->capacity			= capacity;
	q->mask				= capacity - 1;

	q->producer_offset 	= 0;	
	q->consumer_offset 	= 0;
}

bool produce_parallel_queue(ParallelQueue *q, void *elem) {
	atomic_u64 producer_offset = atomic_load_relaxed_u64(&q->producer_offset);
	atomic_u64 consumer_offset = atomic_load_acquire_u64(&q->consumer_offset);

	if (producer_offset - consumer_offset == q->capacity) {
		return false;
	}

	u64 index = (producer_offset & q->mask) * q->elem_size;

	memcpy(&q->buffer[index], elem, q->elem_size);
	atomic_store_release_u64(&q->producer_offset, producer_offset + 1);

	return true;
}

bool consume_parallel_queue(ParallelQueue *q, void *out) {
	atomic_u64 consumer_offset = atomic_load_relaxed_u64(&q->consumer_offset);
	atomic_u64 producer_offset = atomic_load_acquire_u64(&q->producer_offset);

	if (consumer_offset == producer_offset) {
		return false;
	}

	u64 index = (consumer_offset & q->mask) * q->elem_size;

	memcpy(out, &q->buffer[index], q->elem_size);
	atomic_store_release_u64(&q->consumer_offset, consumer_offset + 1);

	return true;
}

bool empty_parallel_queue(ParallelQueue *q) {
	atomic_u64 consumer_offset = atomic_load_relaxed_u64(&q->consumer_offset);
	atomic_u64 producer_offset = atomic_load_relaxed_u64(&q->producer_offset);

	return consumer_offset == producer_offset;
}

void destroy_parallel_queue(ParallelQueue *q) {
	VirtualFree(q->buffer, 0, MEM_RELEASE);
}