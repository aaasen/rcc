/*
 * A basic rectanglular prism.
 * p1 is treated as the closer upper left point and
 * p2 is treated as the further lower right point where
 * the origin is at the close bottom left.
 */

#include <stdlib.h>
#include <math.h>
#include "point.h"

typedef struct rect {
	point p1; /* Corner of prism */
	point p2; /* Opposite corner of prism */
} rect;

double getwr(rect* box);
double gethr(rect* box);
double getdr(rect* box);
double rszaxis(double center, double width, double point);
double rszsum(rect* box, point* p);
int pinr(rect* box, point* p);

/* get the width, height and depth of the rectangle, respectively */
double getwr(rect* box) {
	return fabs(box->p1.x - box->p2.x);
}

double gethr(rect* box) {
	return fabs(box->p1.y - box->p2.y);
}

double getdr(rect* box) {
	return fabs(box->p1.z - box->p2.z);
}

/* returns the amount that the node has to be resized by on one axis */
double rszaxis(double center, double width, double point) {
    int direction = (center - point >= 0) ? 1 : -1;
	double border = center + (direction * width);
	return point - border;
}

/* returns the total amount that the node must be resized to accomodate the given point */
double rszsum(rect* box, point* p) {
	double rszsum = 0;
	double width = getwr(box);
	double height = gethr(box);
	double depth = getdr(box);
	rszsum += abs(rszaxis(box->p1.x + (width / 2), width, p->x));
	rszsum += abs(rszaxis(box->p1.y + (height / 2), height, p->y));
	rszsum += abs(rszaxis(box->p1.z + (depth / 2), depth, p->z));
	return rszsum;
}

/* returns true if the point is inside the rectangle and false (0) if not */
int pinr(rect* box, point* p) {
	return ((box->p1.x <= p->x) &&  (p->x <= box->p2.x) &&
			(box->p1.y >= p->y) &&  (p->y >= box->p2.y) &&
			(box->p1.z <= p->z) &&  (p->z <= box->p2.z)) ? 1 : 0;
}
