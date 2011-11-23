/*
 * Rectangle array class, similar in function to point arrays.
 */

#ifndef RECTARRAY_H
#define RECTARRAY_H

#include <stdlib.h>
#include <stdio.h>
#include "rect.h"

typedef struct rectarray{
	rect* rects;
	int len;
} rectarray;

void addra(rectarray* ra, rect* r); /* Add the rectangle to the rectangle array */

rectarray* createra(); /* Create a new rectangle array */

#endif
