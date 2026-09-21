#ifndef SORT_H
#define SORT_H

void qsort_generic(void *base, int elem_size, int low, int high, int (*cmp)(const void *, const void *));

#endif