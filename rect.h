/*
 * A basic rectanglular prism.
 */

#include <stdlib.h>
#include <math.h>
#include "point.h"

#ifndef RECT_H
#define RECT_H

typedef struct rect {
	point p1; /* Corner of prism */
	point p2; /* Opposite corner of prism */
} rect;

double getwr(rect* box);
double gethr(rect* box);
double getdr(rect* box);

/* returns the amount that the node has to be resized by on one axis */
double rszaxis(double center, double width, double point);

/* returns the total amount that the node must be resized to accomodate the given point */
double rszsum(rect* box, point* p);

#endif
