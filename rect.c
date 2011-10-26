/*
 * A basic rectanglular prism.
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

double getwr(rect* box) {
	return fabs(box->p1.x - box->p2.x);
}

double gethr(rect* box) {
	return fabs(box->p1.y - box->p2.y);
}

double getdr(rect* box) {
	return fabs(box->p1.z - box->p2.z);
}

double rszaxis(double center, double width, double point) {
    int direction = (center - point >= 0) ? 1 : -1;
	double border = center + (direction * width);
	return point - border;
}

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
