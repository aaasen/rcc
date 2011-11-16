/* A basic sphere */

#ifndef SPHERE_H
#define SPHERE_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "point.h"
#include "rect.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

typedef struct sphere {
	point* center;
	double radius;
} sphere;

/* returns true if the given point is in the sphere */
int pinsphr(sphere* sphr, point* p);

/* returns true if the given rectangle and sphere overlap */
int rinsphr(sphere* sphr, rect* r);

/* returns true if the line defined by the given endpoins overlaps the sphere */
int linsphr(sphere* sphr, point* p1, point* p2);

/* finds the smallest rectangular prism which encompasses the sphere */
rect* findmbrsphr(sphere* sphr);

/* returns a pointer to a new sphere with the given parameters */
sphere* createsphr(double x, double y, double z, double radius);

/* prints the sphere */
void printsphr(sphere* sphr);

/* frees the sphere and its nested structs */
void freesphr(sphere* sphr);

#endif
