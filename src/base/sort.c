#include "sort.h"

static void swap(void *a_, void *b_, int elem_size) {
	unsigned char *a = (unsigned char *)a_;
	unsigned char *b = (unsigned char *)b_;

	if (a == b) return;
	
	int i;
	for (i = 0; i < elem_size; ++i) {
		unsigned char t = a[i];
		a[i] = b[i];
		b[i] = t;
	}
}

void qsort_generic(void *base, int elem_size, int low, int high, int (*cmp)(const void *, const void *)) {
	unsigned char *array = base;

	if (!base || !cmp || elem_size == 0 || low < 0) {
        return;
    }

	while (low < high) {
		unsigned char *pivot_ptr = array + high * elem_size;

		int i;
		int j;
		for (i = low, j = low; j < high; ++j) {
			void *aj = array + j * elem_size;
			if (cmp(aj, pivot_ptr) <= 0) {
				void *ai = array + i * elem_size;

				swap(ai, aj, elem_size);
				++i;
			}
		}

		swap(array + i * elem_size, array + high * elem_size, elem_size);

		int l_pivot = i - 1;
		int r_pivot = i + 1;

		if (l_pivot - low < high - r_pivot) {
			qsort_generic(array, elem_size, low, l_pivot, cmp);
			low = r_pivot;
		} else {
			qsort_generic(array, elem_size, r_pivot, high, cmp);
			high = l_pivot;
		}
	}
}