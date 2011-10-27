/*
 * A basic rectanglular prism.
 * p1 is treated as the closer upper left point and
 * p2 is treated as the further lower right point where
 * the origin is at the close bottom left.
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

/* get the width, height and depth of the rectangle, respectively */
double getwr(rect* box);
double gethr(rect* box);
double getdr(rect* box);

/* returns the amount that the node has to be resized by on one axis */
double rszaxis(double center, double width, double point);

/* returns the total amount that the node must be resized to accomodate the given point */
double rszsum(rect* box, point* p);

/* returns true if the point is inside the rectangle and false (0) if not */
int pinr(rect* box, point* p);

/* evaluates whether or not two rectangles overlap and returns true if they do and false if not */
int rinr(rect* box1, rect* box2);

#endif
