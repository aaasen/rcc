/*
 * Rectangle array class, similar in function to point arrays.
 */

#include <stdlib.h>
#include <stdio.h>
#include "rect.h"

typedef struct rectarray{
	rect* rects;
	int len;
} rectarray;

void addra(rectarray* ra, rect* r);
rectarray* createra();

/* Add the rectangle to the rectangle array */
void addra(rectarray* ra, rect* r){
	if (ra && r){
		ra->rects = (rect*) realloc(ra->rects, sizeof(rect) * (ra->len + 1));
		ra->rects[ra->len] = *r;
		ra->len++;
	}
}

/* Create a new rectangle array */
rectarray* createra(){
	rectarray* ra = (rectarray*)malloc(sizeof(rectarray));
	ra->rects = NULL;
	ra->len = 0;
	return ra;
}
