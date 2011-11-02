/*
 * An array of points with a length.
 */

#ifndef PARRAY_H
#define PARRAY_H

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "point.h" 

typedef struct parray {
	size_t len;
	point* points;
} parray;

/* adds a point to the array and returns -1 if any arguments are null */
signed int addpa(parray* pa, point* p);
/* performs a brute force search through the array and removes all matching points and returns -1 if any arguments are null*/
signed int rempa(parray* pa, point* p);
/* returns the number of points in the array (parray.len) */
size_t lenpa(parray* pa);

#endif
