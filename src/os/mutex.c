#include "mutex.h"

#include <windows.h>

Mutex create_mutex() {
	CRITICAL_SECTION *mutex;
	mutex = malloc(sizeof *mutex);
	InitializeCriticalSection(mutex);
	return mutex;
}

void lock_mutex(Mutex mutex) {
	EnterCriticalSection(mutex);
}

bool try_lock_mutex(Mutex mutex) {
	return TryEnterCriticalSection(mutex);
}

void unlock_mutex(Mutex mutex) {
	LeaveCriticalSection(mutex);
}

void destroy_mutex(Mutex mutex) {
	DeleteCriticalSection(mutex);
	free(mutex);
}