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
 *               RTREE COMPRESSION CODEC
 *                          
 *     BY                        
 *-------Lane "Laaame" Aasen
 *       ------Eamon "G-Dawg" Gaffney
 *             ------Michael "Nerdberger" Rosenberger
 *                   ------Dylan "D-Swag" Swiggett
 */


#include <stdlib.h>
#include <stdio.h>
#include <math.h>

static int maxsdev = 4; /* Maximum standard deviation within each rtree */

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

/* Give the point the specified x, y, z */
void setxyz(point* p, double x, double y, double z){
  p->x = x;
  p->y = y;
  p->z = z;
}

/* Add the specified point to the specified rtree */
rtree* putrt(rtree * tree, point * p){
  if (tree->leaf){
    //TODO Add point to points
    //Realloc as necessary
    resizert(tree);
  }
}

/* Efficiently bulk add all of the points in p */
void bputrt(rtree* tree, point* p, int n){
	tree->points = (point*)realloc(tree->points, n + tree->n);
	int i;
	for (i = 0; i < n; i ++){
	  tree->points[i + tree->n] = p[i];
	}
	tree->n += n;
	subrt(tree);
}

/* 
 * Returns the standard deviation of the y (depth) value for the rtree.
 * Returns 0 if the tree is not a leaf - a branch can not and should not be subdivided.
 * Currently used for checking if the tree should be subdivided.
 * max and min are used to find the highest and lowest points in the tree.
 */
double sdevrt(rtree * tree, point * max, point * min){
  if (tree->n == 0 || !(tree->leaf)){
    return 0;
  }
  int i;
  int total = 0;
  point* maxp = &(tree->points[0]);/* Keep track of maximum z value point */
  point* minp = &(tree->points[0]);/* Keep track of minimum z value point */
  for (i = 0; i < tree->n; i++){
    total += tree->points[i].z;
	if (tree->points[i].z > maxp->z){
	  maxp = &(tree->points[i]);
	}
	if (tree->points[i].z < minp->z){
	  minp = &(tree->points[i]);
	}
  }
  if (!(max == NULL || min == NULL)){
	/* Transfer x,y,z for max and min points */
	setxyz(max, maxp->x, maxp->y, maxp->z);
	setxyz(min, minp->x, minp->y, minp->z);
  }
  double mean = total / tree->n;
  double sumSqr = 0;
  for (i = 0; i < tree->n; i++){
    sumSqr += pow(tree->points[i].z - mean, 2);
  }
  return sqrt(sumSqr / (tree->n - 1));
}

/*
 * Subdivide the selected rtree if a leaf and if meets subdivision reqs
 * Returns false if rtree doesn't need to be subdivided under current rule
*/
int subrt(rtree* tree){
  point * max = (point*)malloc(sizeof(point));
  point * min = (point*)malloc(sizeof(point));
  double sdev = sdevrt(tree, max, min);
  if (sdev > maxsdev){
	tree->leaf = 1;
	tree->sub1 = (rtree*)malloc(sizeof(rtree));
	tree->sub2 = (rtree*)malloc(sizeof(rtree));
	/* Give each subtree as much memory as it might need */
	tree->sub1->points = (point*)malloc(sizeof(point) * tree->n);
	tree->sub2->points = (point*)malloc(sizeof(point) * tree->n);
	int i;
	for (i = 0; i < tree.n; i++){
	  /* 
	   * TODO: Add point to appropriate subtree.
	   * Expand as needed.
	   */
	}
	/* Free any unneeded memory in the subtrees, then free the original point array */
	tree->sub1->points = realloc(tree->sub1->points, sizeof(point) * tree->sub1->n);
	tree->sub2->points = realloc(tree->sub2->points, sizeof(point) * tree->sub2->n);
	free(tree->points);
	return 1;
  }
  printf("max z: %f\nmin z: %f\n", max->z, min->z);
  return 0;
}

/* Recursively find and remove the point from the tree */
rtree* remrt(rtree* tree, point * p){
  if (tree->leaf){
    //Check if point is in list, and remove
  } else {
    
  }
}

/* Recursively resize the tree, return false if rebuilding might be necessary */
int resizert(rtree * tree){
  return 0;
}
