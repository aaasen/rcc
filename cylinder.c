/* A basic cylinder */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "point.h"
#include "rect.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

typedef struct {
	point* centbase; /* Center of the base circle */
	double height;
	double radius;
} cylinder;

int pincyl(cylinder* cyl, point* p);
int rincyl(cylinder* cyl, rect* r);
int lintc(point* p1, point* p2, point* center, double radius);
cylinder* createcyl(double x, double y, double z, double height, double radius);
rect* getcylmbr(cylinder* cyl);
void printcyl(cylinder* cyl);
void freecyl(cylinder* cyl);

/* returns true if the given point is in the cylinder */
int pincyl(cylinder* cyl, point* p){
	return distp(cyl->centbase, p) <= cyl->radius && p->z >= cyl->centbase->z && p->z <= cyl->height + cyl->centbase->z;
}

/* returns true if the given rectangle and cylinder overlap */
int rincyl(cylinder* cyl, rect* r){
	if (cyl){
		return rinr(r, getcylmbr(cyl)) /* the minimum bounding rectangle must overlap with the search rectangle */
			   && lintc(&r->min, &r->max, cyl->centbase, cyl->radius);/* check if any lines intersect the circle */
	} return 0;
}

/*
 * checks if a line intersects a circle, line must be parallel to x or y axis, z values are ignored, this funtion is
 * optimized for the rincyl search and probably won't do what you want it to in most other siuations
 */
int lintc(point* p1, point* p2, point* center, double radius){
	return distp(p1, center) <= radius || distp(p2, center) <= radius /* if either endpoint is within the circle */
		   || (min(p1->x, p2->x) <= radius && max(p1->x, p2->x) >= radius) /* if the line goes through the circle */
		   || (min(p1->y, p2->y) <= radius && max(p1->y, p2->y) >= radius); /* the check must be made on both x and y */
}

/* returns a pointer to a new cylinder with the given parameters */
cylinder* createcyl(double x, double y, double z, double height, double radius){
	cylinder* new = malloc(sizeof(cylinder));
	new->centbase = createp(x, y, z);
	new->height = height;
	new->radius = radius;
	return new;
}

/* returns the minimum bounding rectangle of a cylinder */
rect* getcylmbr(cylinder* cyl){
	rect* r = malloc(sizeof(rect));
	r->min = *createp(cyl->centbase->x - cyl->radius, cyl->centbase->y - cyl->radius, cyl->centbase->z);
	r->max = *createp(cyl->centbase->x + cyl->radius, cyl->centbase->y + cyl->radius, cyl->centbase->z + cyl->height);
	return r;
}

/* prints the cylinder */
void printcyl(cylinder* cyl){
	if(cyl) {
		printf("sphere(center%sradius(%.2lf)height(%.2lf))\n", tostringp(cyl->centbase), cyl->radius, cyl->height);
	}
}

/* frees the cylinder and its nested structs */
void freecyl(cylinder* cyl){
	if (cyl) {
		free(cyl->centbase);
		free(cyl);
	}
}

