#ifndef FIXED_QUEUE
#define FIXED_QUEUE

#include "types.h"
#include <stdbool.h>

typedef struct FixedQueue FixedQueue;
struct FixedQueue {
	unsigned char  *buffer;
	int 			elem_size;
	            	
	int 			size;
	int 			capacity;
					
	int 			head;
	int 			tail;
};

void 	init_fixed_queue	(FixedQueue *q, int elem_size, int capacity);
bool 	push_fixed_queue	(FixedQueue *q, void *elem);
void   *pop_fixed_queue		(FixedQueue *q);

bool 	empty_fixed_queue	(FixedQueue *q);
bool 	full_fixed_queue	(FixedQueue *q);

void 	destroy_fixed_queue	(FixedQueue *q);

#endif