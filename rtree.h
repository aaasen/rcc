/*  ______________________
 * |  ____                | * * * * * * * * * * * * *
 * | |    |               |   _         __      __  *
 * | | ___|________       |  ||\\      / _|    / _| *
 * | ||   |        |      |  || \\    / /     / /   *
 * | ||___|  ___   |      |  || //   | |     | |    *
 * |  |     |   |  |      |  ||/\\    \ \_    \ \_  *
 * |  |     |___|  |      |  ||  \\    \__|    \__| *
 * |  |____________|      |                         *
 * |______________________| * * * * * * * * * * * * *
 *
 *	 	 	 	 	RTREE COMPRESSION CODEC
 *
 *	 	BY
 *-------Lane "Laaame" Aasen
 *	 	  ------Eamon "G-Dawg" Gaffney
 *	 	 	 	  ------Michael "Nerdberger" Rosenberger
 *	 	 	 	 	 	  ------Dylan "D-Swag" Swiggett
 *
 * A basic 3-dimensional R-tree, supporting insertion, deletion, resizing, rebuilding
 * and bulk loading of points.
 */

#ifndef RTREE_H
#define RTREE_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "point.h"
#include "rect.h"
#include "parray.h"
#include "sphere.h"

typedef struct rtree{
	parray pa; /* array of points in the rtree */

	rect mbr; /* Maximum Bounding Rectangle of the node */

	int leaf;/* True if this tree is a leaf, false if a branch */
	/* Possibly make into an n-child rtree, if faster */
	struct rtree * sub1;/* First sub tree if a leaf node */
	struct rtree * sub2;/* Second sub tree if a leaf node */

} rtree;

rtree* putrt(rtree * tree, point * p);/* Add the specified point to the specified rtree */

/* Efficiently bulk add all of the points in pa. */
/* Fails if tree is not a leaf. */
void bputrt(rtree* tree, parray* pa);

rtree* remrt(rtree * tree, point * p);/* Recursively find and remove the point from the tree */

double sdevrt(rtree * tree, point * max, point * min);/* Returns the standard deviation of the y value for the tree */

int subrt(rtree * tree);/* Subdivide the selected rtree if a leaf and if meets subdivision reqs */

int resizert(rtree * tree);/* Recursively resize the tree, return false if rebuilding might be necessary */

void rebuildrt(rtree * tree);/* Recursively rebuild the entire tree, optimizing search time */

rtree* pfindrt(rtree * tree, point * p);/* Recursively search through the rtree to find the rtree containing the specified point. Return null if the point is not in the tree. Assumes the rtree is properly resized */

/* searchrt() is an extensible search function for rtrees */
/* it returns an array of points which are contained in qshape (which can be any, preferably geometric, struct) */
/* searchrt() requires two function pointers: */
/* 		-pinshape(), or point in shape, takes a struct which is the same type as qshape and a point* as arguments */
/*			and returns 1 if the point is in the shape and anything else if it is not */
/*		-rinshape(), or rectangle in shape, is identical in behavior to pinshape() */
/*			but takes a rect* instead of a point* as its second argument */
/* using these functions searchrt() recursively traverses the rtree and compiles an array of points */
/* which are enclosed by the given shape */
parray* searchrt(rtree* tree, void* qshape, int (pinshape)(void*, point*), int (rinshape)(void*, rect*));

/* returns an array of points which are in the query rectangle */
parray* rsearchrt(rtree* tree, rect* qbox);

/* returns an array of points which are in the query sphere */
parray* sphrsearchrt(rtree* tree, sphere* qrysphere);

/* recursively builds a parray containing the points in the given node and in all of its children */
parray* getpointsrt(rtree* tree);

/* Allocate and assign everything necessary for a new leaf rtree */
rtree* creatert();

/* Recursively print all points in the rtree */
void printrt(rtree* tree);

/* Recursively free the rtree and all of its nodes */
void freert(rtree* tree);

#endif
