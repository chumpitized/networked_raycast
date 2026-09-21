#ifndef COND_VARIABLE_H
#define COND_VARIABLE_H

#include <stdbool.h>
#include <windows.h>

typedef CONDITION_VARIABLE CV;

CV	   *create_cv		();
int 	cv_wait			(CV *cv, void *mutex);
int 	cv_signal_one	(CV *cv);
int 	cv_signal_all	(CV *cv);
void 	cv_destroy		(CV *cv);

#endif