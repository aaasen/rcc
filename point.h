/*
 * A basic 3D point.
 */

#ifndef POINT_H
#define POINT_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct point{
	double x, y, z;
} point;

/* creates a new point */
point* createp(double x, double y, double z);

void setxyz(point* p, double x, double y, double z);/* Give the point the specified x, y, z */

int peq(point* p1, point* p2); /* returns true if points are equal, false if not */

/* returns true if the value is in between the specified value */
/* if inclusive is true <= will be used; < will be used if not */
int pinrng(double lower, double upper, double p, int inclusive);

/* calculates the distance between two points */
double distp(point* p1, point* p2);

/* returns a string representation of the point in standard coordinate format */
char* tostringp(point* p);

#endif
