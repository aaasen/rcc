/*
 * A basic rectanglular prism.
 * p1 is treated as the closer lower left point (lower values)and
 * p2 is treated as the further upper right point (higher values)
 * where the origin is at the close bottom left.
 */

#include <stdlib.h>
#include <math.h>
#include "point.h"
#include "parray.h"

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
int rinr(rect* box1, rect* box2);
rect* findmbr(parray* pa);
void printrect(rect* box);

/* get the width, height and depth of the rectangle, respectively */
double getwr(rect* box) {
	return box->p2.x - box->p1.x;
}

double gethr(rect* box) {
	return box->p2.y - box->p1.y;
}

double getdr(rect* box) {
	return box->p2.z - box->p1.z;
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
	return ((pinrng(box->p1.x, box->p2.x, p->x, 1)) &&
		(pinrng(box->p1.y, box->p2.y, p->y, 1)) &&
		(pinrng(box->p1.z, box->p2.z, p->z, 1)));
}

/* evaluates whether or not two rectangles overlap and returns true if they do and false if not */
int rinr(rect* box1, rect* box2) {
	return pinr(box1, &box2->p1) || pinr(box1, &box2->p2) || pinr(box2, &box1->p1) || pinr(box2, &box1->p2);
}

/* calculates the minimum bounding rectangle (mbr) of the set of points */
rect* findmbr(parray* pa) {
	int i;
	rect* mbr = (rect*) malloc(sizeof(rect));
	
	if(pa && pa->len > 0) {		
		mbr->p1 = pa->points[0];
		mbr->p2 = pa->points[0];	
		for(i = 0; i < pa->len; i++) {
			if(pa->points[i].x < mbr->p1.x) mbr->p1.x = pa->points[i].x;
			else if(pa->points[i].x > mbr->p2.x) mbr->p2.x = pa->points[i].x;
			
			if(pa->points[i].y < mbr->p1.y) mbr->p1.y = pa->points[i].y;
			else if(pa->points[i].y > mbr->p2.y) mbr->p2.y = pa->points[i].y;
			
			if(pa->points[i].z < mbr->p1.z) mbr->p1.z = pa->points[i].z;
			else if(pa->points[i].z > mbr->p2.z) mbr->p2.z = pa->points[i].z;			
		}
		return mbr;
	}
	return NULL;
}

/* prints both coordinates of the rectangle */
void printrect(rect* box) {
	printf("p1: %s\np2: %s\n", tostringp(&box->p1), tostringp(&box->p2));
}

