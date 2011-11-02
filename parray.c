/*
 * An array of points with a length.
 */
 
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "point.h" 

typedef struct parray {
	size_t len;
	point* points;
} parray;

signed int addpa(parray* pa, point* p);
signed int rempa(parray* pa, point* p);
size_t lenpa(parray* pa);

/* adds a point to the array */
signed int addpa(parray* pa, point* p) {
	if(pa && p) {
		pa->points = (point*)realloc(pa->points, sizeof(point) * ++pa->len);
		pa->points[pa->len - 1] = *p;
		return 1;
	}
	return -1;
}

/* performs a brute force search through the array and removes all matching points */
signed int rempa(parray* pa, point* p) {
	unsigned int i;
	if(pa && p) {
		for(i = 0; i < pa->len; i++) {
			if(peq(p, &pa->points[i])) {
				memmove(&pa->points[i], &pa->points[--pa->len], sizeof(point));
				pa->points = realloc(pa->points, sizeof(point) * pa->len);
			}
		}
		return 1;
	}
	return -1;
}

/* returns the number of points in the array (parray.len) */
size_t lenpa(parray* pa) {
	return pa->len;
}

