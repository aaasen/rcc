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

typedef struct interval {
	double p1; /* First point in interval */
	double p2; /* Second point in interval */
} interval;

double getwr(rect* box);
double gethr(rect* box);
double getdr(rect* box);
double rszaxis(double center, double width, double point);
double rszsum(rect* box, point* p);
int pinr(rect* box, point* p);
int linr(rect* box, point* p1, point* p2);
int rinr(rect* box1, rect* box2);
rect* findmbr(parray* pa);
rect* createrect(double x1, double y1, double z1, double x2, double y2, double z2);
interval* createinterval(double p1, double p2);
void printrect(rect* box);
void freerect(rect* box);

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
	rszsum += abs(rszaxis(box->p1.z + (depth / 2), depth, p->z)) * 0 / 255;/* Z axis has less influence */
	return rszsum;
}

/* returns true if the point is inside the rectangle and false (0) if not */
int pinr(rect* box, point* p) {
	return ((pinrng(box->p1.x, box->p2.x, p->x, 1)) &&
		(pinrng(box->p1.y, box->p2.y, p->y, 1)) &&
		(pinrng(box->p1.z, box->p2.z, p->z, 1)));
}

/* returns true if the line defined by the two poins intersects the rectangle */
int linr(rect* box, point* p1, point* p2){
	return (pinr(box, p1) || pinr(box, p2)) ||
	(p1-> x >= box->p1.x && p2->x <= box->p1.x) ||
	(p2-> x >= box->p1.x && p1->x <= box->p1.x) ||
	(p1-> y >= box->p1.y && p2->y <= box->p1.y) ||
	(p2-> y >= box->p1.y && p1->y <= box->p1.y) ||
	(p1-> z >= box->p1.z && p2->z <= box->p1.z) ||
	(p2-> z >= box->p1.z && p1->z <= box->p1.z);
}

/* evaluates whether or not two rectangles overlap and returns true if they do and false if not */
int rinr(rect* box1, rect* box2) {
	/* Check for intersection !!!! */
	if (box1->p1.x <= box2->p2.x && box1->p2.x >= box2->p1.x &&
	    box1->p1.y <= box2->p2.y && box1->p2.y >= box2->p1.y &&
	    box1->p1.z <= box2->p2.z && box1->p2.z >= box2->p1.z){
		return 1;
	} else {
		return 0;
	}
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

/* returns a pointer to a rectangle which is creating with the given arguments */
rect* createrect(double x1, double y1, double z1, double x2, double y2, double z2) {
	rect* newrect = (rect*) malloc(sizeof(rect));

	newrect->p1 = *createp(x1, y1, z1);
	newrect->p2 = *createp(x2, y2, z2);
	return newrect;
}

/* Create new interval with the specified values */
interval* createinterval(double p1, double p2){
	interval* ivl = (interval*)malloc(sizeof(interval));
	ivl->p1 = p1;
	ivl->p2 = p2;
	return ivl;
}


/* prints both coordinates of the rectangle */
void printrect(rect* box){
	printf("rect(p1%sp2%s)\n", tostringp(&box->p1), tostringp(&box->p2));
}

/* frees the rectangle and all substructs */
void freerect(rect* box) {
	free(&box->p1);
	free(&box->p2);
	free(box);
}

/* finds the inner interval of the two intervals */
interval* innerinterval(interval* inv1, interval* inv2){
	double points[] = {inv1->p1, inv1->p2, inv2->p1, inv2->p2};
	int i, j;
	interval* ivl = (interval*)malloc(sizeof(interval));
	int lesscount, samecount;
	
	for (i = 0; i < 4; i ++){
		lesscount = 0;
		samecount = -1;
		for (j = 0; j < 4; j ++){
			if(points[j] == points[i]){
				samecount++;
			} else if (points[j] < points[i]){
				lesscount++;
			}
		}
		if (lesscount <= 1 && samecount + lesscount >= 1){
			ivl->p1 = points[i];
		}
		if (lesscount <= 2 && samecount + lesscount >= 2){
			ivl->p2 = points[i];
		}
	}
	return ivl;
}

/* finds the inner rectangle of the two rectangles */
rect* innerrect(rect* r1, rect* r2){
	interval* r1x, *r1y, *r1z, *r2x, *r2y, *r2z;
	interval* x, *y, *z;

	r1x = createinterval(r1->p1.x, r1->p2.x);
	r1y = createinterval(r1->p1.y, r1->p2.y);
	r1z = createinterval(r1->p1.z, r1->p2.z);
	r2x = createinterval(r2->p1.x, r2->p2.x);
	r2y = createinterval(r2->p1.y, r2->p2.y);
	r2z = createinterval(r2->p1.z, r2->p2.z);
	
	x = innerinterval(r1x, r2x);
	y = innerinterval(r1y, r2y);
	z = innerinterval(r1z, r2z);

	return createrect(x->p1, y->p1, z->p1, x->p2, y->p2, z->p2);
}

