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
  double x, y, z;/* Corner of prism */
  double x1, y1, z1;/* Opposite corner of prism */
  int leaf;/* True if this tree is a leaf, false if a branch */
  /* Possibly make into an n-child rtree, if faster */
  struct rtree * sub1;/* First sub tree if a leaf node */
  struct rtree * sub2;/* Second sub tree if a leaf node */
} rtree;

rtree* addrt(rtree * tree, point * p);/* Add the specified point to the specified rtree */

rtree* remrt(rtree * tree, point * p);/* Recursively find and remove the point from the tree */

rtree* findrt(rtree * tree, point * p);/* Recursively find the specified point in the tree */

int subrt(rtree * tree);/* Subdivide the selected rtree if a leaf and if meets subdivision reqs */

int resizert(rtree * tree);/* Recursively resize the tree, return false if rebuilding might be necessary */

void rebuildrt(rtree * tree);/* Recursively rebuild the entire tree, optimizing search time */

void bulkrt(rtree * tree, point * p, int n);/* Efficiently bulk add all of the points in p */

#endif
