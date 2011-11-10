/* A basic cylinder */

#ifndef CYLINDER_H
#define CYLINDER_H

#include <stdlib.h>
#include <stdio.h>
#include "point.h"
#include "rect.h"

typedef struct {
	point* centbase; /* Center of the base circle */
	double height;
	double radius;
} cylinder;

/* returns true if the given point is in the cylinder */
int pincyl(cylinder* cyl, point* p);

/* returns true if the given rectangle and cylinder overlap */
int rincyl(cylinder* cyl, rect* r);

/*
 * checks if a line intersects a circle, line must be parallel to x or y axis, z values are ignored, this funtion is
 * optimized for the cilyinder search and probably won't do what you want it to in most other siuations
 */
int lintc(point* p1, point* p2, point* center, double radius);

/* returns a pointer to a new cylinder with the given parameters */
cylinder* createcyl(double x, double y, double z, double height, double radius);

/* returns the minimum bounding rectangle of a cylinder */
rect* getcylmbr(cylinder* cyl);

/* prints the cylinder */
void printcyl(cylinder* cyl);

/* frees the cylinder and its nested structs */
void freecyl(cylinder* cyl);

#endif
