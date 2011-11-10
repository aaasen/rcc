/* A basic sphere */

#include <stdlib.h>
#include <stdio.h>
#include "point.h"
#include "rect.h"

typedef struct sphere {
	point* center;
	double radius;
} sphere;

int pinsphr(sphere* sphr, point* p);
sphere* createsphr(double x, double y, double z, double radius);
void printsphr(sphere* sphr);
void freesphr(sphere* sphr);

/* returns true if the given point is in the sphere */
int pinsphr(sphere* sphr, point* p) {
	if(sphr && p) {
		return (distp(sphr->center, p) <= sphr->radius);
	} return 0;
}

/* returns true if the given rectangle and sphere overlap */
int rinsphr(sphere* sphr, rect* r) {
	if(sphr && r) {
		
	} return 0;
}

/* returns a pointer to a new sphere with the given parameters */
sphere* createsphr(double x, double y, double z, double radius) {
	sphere* newsphr = (sphere*) malloc(sizeof(sphere));

	newsphr->center = createp(x, y, z);
	newsphr->radius = radius;
	return newsphr;
}

/* prints the sphere */
void printsphr(sphere* sphr) {
	if(sphr) {
		printf("sphere(center%sradius(%.2lf))\n", tostringp(sphr->center), sphr->radius);
	}
}
 
/* frees the sphere and its nested structs */
void freesphr(sphere* sphr) {
	if(sphr) {
		free(sphr->center);
		free(sphr);
	}
}
