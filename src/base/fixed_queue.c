#include <windows.h>
#include <assert.h>

#include "fixed_queue.h"

//ideally these take some backing memory, like an arena...
void init_fixed_queue(FixedQueue *q, int elem_size, int capacity) {
	q->buffer 		= (unsigned char *)VirtualAlloc(NULL, capacity * elem_size, MEM_COMMIT, PAGE_READWRITE);
	q->capacity		= capacity;
	q->elem_size 	= elem_size;
	q->size 		= 0;
	q->head 		= 0;
	q->tail 		= 0;
}

bool push_fixed_queue(FixedQueue *q, void *elem) {
	if (q->size == q->capacity) {
		assert("Circular buffer is full!" && 0);
		return false;
	}

	memcpy(&q->buffer[q->head * q->elem_size], elem, q->elem_size);
	q->head = (q->head + 1) % q->capacity;
	q->size++;

	return true;
}

void *pop_fixed_queue(FixedQueue *q) {
	if (q->size == 0) {
		assert("Circular buffer is empty!" && 0);
		return NULL;
	}

	void *res = &q->buffer[q->tail * q->elem_size];
	q->tail = (q->tail + 1) % q->capacity;
	q->size--;

	return res;
}

bool empty_fixed_queue(FixedQueue *q) {
	return q->size == 0;
}

bool full_fixed_queue(FixedQueue *q) {
	return q->size == q->capacity;
}

void destroy_fixed_queue(FixedQueue *q) {
	VirtualFree(q->buffer, 0, MEM_RELEASE);
}