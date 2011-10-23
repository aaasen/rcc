/*
 * A basic 3-dimensional R-tree, supporting insertion, deletion, resizing, rebuilding
 * and bulk loading of points.
 */

#ifndef RTREE_H
#define RTREE_H

#include <stdlib.h>

typedef struct point{
	double x, y, z;
} point;

typedef struct rtree{
	int n;/* Number of points in the section of a tree */
	point * points;/* Array of points within the rtree */
	/*
	 * Should points be used to represent the corners?
	 */
	point p1; //double x, y, z;/* Corner of prism */
	point p2; //double x1, y1, z1;/* Opposite corner of prism */
	int leaf;/* True if this tree is a leaf, false if a branch */
	/* Possibly make into an n-child rtree, if faster */
	struct rtree * sub1;/* First sub tree if a leaf node */
	struct rtree * sub2;/* Second sub tree if a leaf node */
} rtree;

/* get the width, height and depth of the rtree */
double getwrt(rtree* tree);
double gethrt(rtree* tree);
double getdrt(rtree* tree);

void setxyz(point* p, double x, double y, double z);/* Give the point the specified x, y, z */

int peq(point* p1, point* p2); /* returns true if points are equal, false if not */

rtree* putrt(rtree * tree, point * p);/* Add the specified point to the specified rtree */

/* returns the amount that the node has to be resized by on one axis */
double rszaxis(double center, double width, double point);

/* returns the total amount that the node must be resized to accomodate the given point */
double rszsum(rtree* tree, point* p);

void bputrt(rtree * tree, point * p, int n);/* Efficiently bulk add all of the points in p */

rtree* remrt(rtree * tree, point * p);/* Recursively find and remove the point from the tree */

double sdevrt(rtree * tree, point * max, point * min);/* Returns the standard deviation of the y value for the tree */

int subrt(rtree * tree);/* Subdivide the selected rtree if a leaf and if meets subdivision reqs */

int resizert(rtree * tree);/* Recursively resize the tree, return false if rebuilding might be necessary */

void rebuildrt(rtree * tree);/* Recursively rebuild the entire tree, optimizing search time */

#endif
