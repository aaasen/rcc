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

typedef struct rtree{
	int n;/* Number of points in the section of a tree */
	point * points;/* Array of points within the rtree */
	/*
	 * Should points be used to represent the corners?
	 */

	rect mbr; /* Maximum Bounding Rectangle of the node */

	int leaf;/* True if this tree is a leaf, false if a branch */
	/* Possibly make into an n-child rtree, if faster */
	struct rtree * sub1;/* First sub tree if a leaf node */
	struct rtree * sub2;/* Second sub tree if a leaf node */
} rtree;

rtree* putrt(rtree * tree, point * p);/* Add the specified point to the specified rtree */

void bputrt(rtree * tree, point * p, int n);/* Efficiently bulk add all of the points in p */

rtree* remrt(rtree * tree, point * p);/* Recursively find and remove the point from the tree */

double sdevrt(rtree * tree, point * max, point * min);/* Returns the standard deviation of the y value for the tree */

int subrt(rtree * tree);/* Subdivide the selected rtree if a leaf and if meets subdivision reqs */

int resizert(rtree * tree);/* Recursively resize the tree, return false if rebuilding might be necessary */

void rebuildrt(rtree * tree);/* Recursively rebuild the entire tree, optimizing search time */

rtree* pfindrt(rtree * tree, point * p);/* Recursively search through the rtree to find the rtree containing the specified point. Return null if the point is not in the tree. Assumes the rtree is properly resized */

#endif
