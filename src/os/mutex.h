#ifndef MUTEX_H
#define MUTEX_H

#include <stdbool.h>
#include <windows.h>

typedef HANDLE Mutex;

Mutex 	create_mutex	();
void 	lock_mutex		(Mutex mutex);
bool 	try_lock_mutex	(Mutex mutex);
void 	unlock_mutex	(Mutex mutex);
void 	destroy_mutex	(Mutex mutex);

#endif