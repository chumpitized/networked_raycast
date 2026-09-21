#include "cond_variable.h"

#include <windows.h>

CV *create_cv() {
	CONDITION_VARIABLE *cv;
	cv = malloc(sizeof *cv);
	InitializeConditionVariable(cv);
	return cv;
}

int cv_wait(CV *cv, void *mutex) {
	SleepConditionVariableCS(cv, mutex, INFINITE);
	return 0;
}

int cv_signal_one(CV *cv) {
	WakeConditionVariable(cv);
	return 0;
}

int cv_signal_all(CV *cv) {
	WakeAllConditionVariable(cv);
	return 0;
}

void cv_destroy(CV *cv) {	
	free(cv);
}