/*
 * An array of points with a length.
 */

#ifndef PARRAY_H
#define PARRAY_H

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "point.h"

typedef struct parray {
	int len;
	point* points;
} parray;

/* adds a point to the array and returns -1 if any arguments are null */
signed int addpa(parray* pa, point* p);

/* performs a brute force search through the array and removes all matching points and returns -1 if any arguments are null*/
signed int rempa(parray* pa, point* p);

/* returns a parray that contains the contents of both arguments */
parray* mergepa(parray* pa1, parray* pa2);

/* Return the average z value */
double avgzpa(parray* pa);

/* return an initialized parray */
parray* createpa();

/* prints the coordinates of all points in the parray */
void printpa(parray* pa);

/* frees the parray and its substructs */
void freepa(parray* pa);

#endif
