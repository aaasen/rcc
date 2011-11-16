/* A basic sphere */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "point.h"
#include "rect.h"

#define amin(a, b) ((abs(a) < abs(b)) ? (a) : (b))
#define amax(a, b) ((abs(a) > abs(b)) ? (a) : (b))

typedef struct sphere {
	point* center;
	double radius;
} sphere;

int pinsphr(sphere* sphr, point* p);
int rinsphr(sphere* sphr, rect* r);
int linsphr(sphere* sphr, point* p1, point* p2);
rect* findmbrsphr(sphere* sphr);
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
	point *close, *far; /* closest and furthest rectangle corners from sphere center */
	if (sphr && r) {
		/* Heuristic to narrow down the number of cases that need to be fully tested */
		if (rinr(findmbrsphr(sphr), r)) {
			if (pinr(r, sphr->center)) {
				/* Case 1: center of sphere in rectangle */
				return 1;
			} else {
				printf("hit case 2\n");
				/* Case 2: at least one edge of rectangle intersects the sphere */
				/* TODO: Make this only return when it's supposed to */
				close = malloc(sizeof(point));
				far = malloc(sizeof(point));
				/* Not using createp here to add readability */
				close->x = close->x = amin(r->p1.x - sphr->center->x, r->p2.x - sphr->center->x) + sphr->center->x;
				close->x = close->y = amin(r->p1.y - sphr->center->y, r->p2.y - sphr->center->y) + sphr->center->y;
				close->z = close->z = amin(r->p1.z - sphr->center->z, r->p2.z - sphr->center->z) + sphr->center->z;
				far->x = close->x = amax(r->p1.x - sphr->center->x, r->p2.x - sphr->center->x) + sphr->center->x;
				far->x = close->y = amax(r->p1.y - sphr->center->y, r->p2.y - sphr->center->y) + sphr->center->y;
				far->z = close->z = amax(r->p1.z - sphr->center->z, r->p2.z - sphr->center->z) + sphr->center->z;
				/* only need to test the three lines connected to the corner closest to the center of the sphere */
				/* give them each their own if to save the compiler time if one intersects */
				if (linsphr(sphr, close, createp(far->x, close->y, close->z))) {
					return 1;
				} else if (linsphr(sphr, close, createp(close->x, far->y, close->z))) {
					return 1;
				} else if (linsphr(sphr, close, createp(close->x, close->y, far->z))) {
					return 1;
				}
		
				printf("hit case 3\n");
				/* Case 3: a line extending from the center of the sphere paralell */
				/* to one of the original axes will intersect the rectangle */
				if (linr(r, createp(sphr->center->x + sphr->radius, sphr->center->y, sphr->center->z),
						 createp(sphr->center->x - sphr->radius, sphr->center->y, sphr->center->z))) {
					return 1;
				} else if (linr(r, createp(sphr->center->x, sphr->center->y + sphr->radius, sphr->center->z),
						 createp(sphr->center->x, sphr->center->y - sphr->radius, sphr->center->z))) {
					return 1;
				} else if (linr(r, createp(sphr->center->x, sphr->center->y, sphr->center->z + sphr->radius),
						 createp(sphr->center->x, sphr->center->y, sphr->center->z - sphr->radius))) {
					return 1;
				}
			}
		}
	} return 0;	
}

/* Checks if a line intersects a sphere */
int linsphr(sphere* sphr, point* p1, point* p2){
	point* close;
	close = malloc(sizeof(point));
	if (p1->x == p2->x) {
		close->x = p1->x;
	} else {
		if ((p1->x > sphr->center->x && p2->x < sphr->center->x) ||
		   (p1->x < sphr->center->x && p2->x > sphr->center->x)) {
			/* If the center's x coordinate lies within the line */
			close->x = sphr->center->x;
		} else {
			close->x = amin(p1->x - sphr->center->x, p2->x - sphr->center->x) + sphr->center->x;
		}
	}
	if (p1->y == p2->y) {
		close->y = p1->y;
	} else {
		if ((p1->y > sphr->center->y && p2->y < sphr->center->y) ||
		   (p1->y < sphr->center->y && p2->y > sphr->center->y)) {
			close->y = sphr->center->y;
		} else {
			close->y = amin(p1->y - sphr->center->y, p2->y - sphr->center->y) + sphr->center->y;
		}
	}
	if (p1->z == p2->z) {
		close->z = p1->z;
	} else {
		if ((p1->z > sphr->center->z && p2->z < sphr->center->z) ||
		   (p1->z < sphr->center->z && p2->z > sphr->center->z)) {
			close->z = sphr->center->z;
		} else {
			close->z = amin(p1->z - sphr->center->z, p2->z - sphr->center->z) + sphr->center->z;
		}
	}
	return pinsphr(sphr, close);
}

/* finds the smallest rectangular prism which encompasses the sphere */
rect* findmbrsphr(sphere* sphr) {
	rect* mbr;
	
	if(sphr) {
		mbr = createrect(sphr->center->x - sphr->radius, sphr->center->y - sphr->radius, sphr->center->z - sphr->radius,
			sphr->center->x + sphr->radius, sphr->center->y + sphr->radius, sphr->center->y + sphr->radius);
		return mbr;
	}
	return NULL;
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
