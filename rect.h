/*
 * A basic rectanglular prism.
 * p1 is treated as the closer upper left point and
 * p2 is treated as the further lower right point where
 * the origin is at the close bottom left.
 */

#include <stdlib.h>
#include <math.h>
#include "point.h"
#include "parray.h"

#ifndef RECT_H
#define RECT_H

typedef struct rect {
	point min; /* Corner of prism */
	point max; /* Opposite corner of prism */
} rect;

typedef struct interval {
	double p1; /* First point in interval */
	double p2; /* Second point in interval */
} interval;

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

/* returns true if the line defined by the two poins intersects the rectangle */
int linr(rect* box, point* p1, point* p2);

/* evaluates whether or not two rectangles overlap and returns true if they do and false if not */
int rinr(rect* box1, rect* box2);

/* calculates the minimum bounding rectangle (mbr) of the set of points */
rect* findmbr(parray* pa);

/* returns a pointer to a rectangle which is creating with the given arguments */
rect* createrect(double x1, double y1, double z1, double x2, double y2, double z2);

/* Create new interval with the specified values */
interval* createinterval(double p1, double p2);

/* prints both coordinates of the rectangle */
void printrect(rect* box);

/* frees the rectangle and all substructs */
void freerect(rect* box);

/* finds the inner interval of the two intervals */
interval* innerinterval(interval* inv1, interval* inv2);

/* finds the inner rectangle of the two rectangles */
rect* innerrect(rect* r1, rect* r2);

#endif
