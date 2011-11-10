/* A basic cylinder */

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
int pincyl(cylinder* cyl, point* p){
	return distp(cyl->centbase, p) <= cyl->radius && p->z >= cyl->centbase->z && p->z <= cyl->height + cyl->centbase->z;
}

/*/* returns true if the given rectangle and cylinder overlap */
/*int rincyl(cylinder* cyl, rect* r){*/
/*	if (cyl){*/
/*		return rinr(r, getcylmbr(cyl)) /* the minimum bounding rectangle must overlap with the search rectangle */
/*			   && (lintc(r->p1.x, r->p2.x, cyl->centbase, cyl->radius) /* check if any lines intersect the circle */
/*			       || lintc(r->p1.x, r->p2.x, cyl->centbase, cyl->radius));*/
/*	} return 0;*/
/*}*/

/*
 * checks if a line intersects a circle, line must be parallel to x or y axis, z values are ignored, this funtion is
 * optimized for the rincyl search and probably won't do what you want it to in most other siuations
 */
int lintc(point* p1, point* p2, point* center, double radius){
	return distp(p1, center) <= radius || distp(p2, center) <= radius /* if either endpoint is within the circle */
		   || (Min(p1->x, p2->x) <= radius && Max(p1->x, p2->x) >= radius) /* if the line goes through the circle */
		   || (Min(p1->y, p2->y) <= radius && Max(p1->y, p2->y) >= radius); /* the check must be made on both x and y */
}

/* returns a pointer to a new cylinder with the given parameters */
cylinder* createcyl(double x, double y, double z, double height, double radius){
	cylinder* new = malloc(sizeof(cylinder));
	new->centbase = createp(x, y, z);
	new->height = height;
	new->radius = radius;
	return new;
}

/*/* returns the minimum bounding rectangle of a cylinder */
/*rect* getcylmbr(cylinder* cyl){*/
/*	rect* r = malloc(sizeof(rect));*/
/*	r->p1 = *createp(cyl->centbase->x - cyl->radius, cyl->centbase->y - cyl->radius, cyl->centbase->z);*/
/*	r->p2 = *createp(cyl->centbase->x + cyl->radius, cyl->centbase->y + cyl->radius, cyl->centbase->z + cyl->height);*/
/*	return r;*/
/*}*/

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

/* Testing code to put in main */
/*	rect* r1 = malloc(sizeof(rect));*/
/*	rect* r2 = malloc(sizeof(rect));*/
/*	point* p1 = createp(1.0, 1.0, 1.0);*/
/*	point* p2 = createp(10.0, 10.0, 10.0);*/
/*	cylinder* cyl = createcyl(1.0, 1.0, 0.0, 2.0, 1.0);*/
/*	r1->p1 = *createp(0.1, 0.1, 0.0);*/
/*	r1->p2 = *createp(1.9, 1.9, 1.0);*/
/*	r2->p1 = *createp(0.1, 0.1, 0.0);*/
/*	r2->p2 = *createp(0.2, 0.2, 1.0);*/
/*	printf("1:[%i],[%i], 0:[%i],[%i]", pincyl(cyl, p1), rincyl(cyl, r1), pincyl(cyl, p2), rincyl(cyl, r2));*/
