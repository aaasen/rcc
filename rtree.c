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

double dabs(double num);
double getwrt(rtree* tree);
double gethrt(rtree* tree);
double getdrt(rtree* tree);
void setxyz(point* p, double x, double y, double z);
rtree* putrt(rtree * tree, point * p);
double rszaxis(double center, double width, double point);
double rszsum(rtree* tree, point* p);
void bputrt(rtree * tree, point * p, int n);
rtree* remrt(rtree * tree, point * p);
double sdevrt(rtree * tree, point * max, point * min);
int subrt(rtree * tree);
int resizert(rtree * tree);
void rebuildrt(rtree * tree);

double dabs(double num) {
	return num >= 0 ? num : -num;
}

double getwrt(rtree* tree) {
	return dabs(tree->p1.x - tree->p2.x);
}

double gethrt(rtree* tree) {
	return dabs(tree->p1.y - tree->p2.y);
}

double getdrt(rtree* tree) {
	return dabs(tree->p1.z - tree->p2.z);
}

/* Give the point the specified x, y, z */
void setxyz(point* p, double x, double y, double z){
	p->x = x;
	p->y = y;
	p->z = z;
}

/* Add the specified point to the specified rtree
 * Can not resize the rtree, only expand it. e.g. a large prism has been predefined
 * so that all points that fall within it will be put in it. After a single point
 * is inserted, the tree should NOT be shrunk to only fit that point.
 */
rtree* putrt(rtree * tree, point * p){
	if (tree->sub1 == NULL && tree->sub2 == NULL){
		tree->leaf = 1;
	}
	if (p != NULL){
		if (tree->leaf){
			//TODO Add point to points
			//Realloc as necessary
			tree->n++;
			tree->points = (point*)realloc(tree->points, sizeof(point) * tree->n);
			tree->points[tree->n - 1] = *p;
		} else {
			/* Select the subtree that can contain the point with the least expansion, or
			 * if both require the same expansion, add to the first.
			 */
			double rszsub1 = rszsum(tree->sub1, p);
			double rszsub2 = rszsum(tree->sub2, p);
			putrt(rszsub1 >= rszsub2 ? tree->sub1 : tree->sub2, p);
		}
	}
}

double rszaxis(double center, double width, double point) {
    int direction = (center - point >= 0) ? 1 : -1;
	double border = center + (direction * width);
	return point - border;
}

double rszsum(rtree* tree, point* p) {
	double rszsum = 0;
	double width = getwrt(tree);
	double height = gethrt(tree);
	double depth = getdrt(tree);
	rszsum += abs(rszaxis(tree->p1.x + (width / 2), width, p->x));
	rszsum += abs(rszaxis(tree->p1.y + (height / 2), height, p->y));
	rszsum += abs(rszaxis(tree->p1.z + (depth / 2), depth, p->z));
	return rszsum;
}

/* Efficiently bulk add all of the points in p */
void bputrt(rtree* tree, point* p, int n){
	int i;

	tree->points = (point*)realloc(tree->points, n + tree->n);
	for (i = 0; i < n; i ++){
		tree->points[i + tree->n] = p[i];
	}
	tree->n += n;
	subrt(tree);
}

/* Recursively find and remove the point from the tree */
rtree* remrt(rtree* tree, point * p){
	if (tree->leaf){
		//Check if point is in list, and remove
	} else {
		
	}
}

/* 
 * Returns the standard deviation of the y (depth) value for the rtree.
 * Returns 0 if the tree is not a leaf - a branch can not and should not be subdivided.
 * Currently used for checking if the tree should be subdivided.
 * max and min are used to find the highest and lowest points in the tree.
 */
double sdevrt(rtree * tree, point * max, point * min){
	int i;
	int total;
	point* maxp;
	point* minp;
	double mean;
	double sumsqr;

	if (tree->n <= 1 || !(tree->leaf)){
		return 0;
	}
	total = 0;
	maxp = &(tree->points[0]);/* Keep track of maximum z value point */
	minp = &(tree->points[0]);/* Keep track of minimum z value point */
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
	mean = total / tree->n;
	sumsqr = 0;
	for (i = 0; i < tree->n; i++){
	  sumsqr += pow(tree->points[i].z - mean, 2);
	}
	return sqrt(sumsqr / (tree->n - 1));
}

/*
 * Subdivide the selected rtree if a leaf and if meets subdivision reqs
 * Returns false if rtree doesn't need to be subdivided under current rule
*/
int subrt(rtree* tree){
	point* max;
	point* min;
	double sdev;
	int i;
	double sum1;
	double sum2;

	max = (point*)malloc(sizeof(point));
	min = (point*)malloc(sizeof(point));
	sdev = sdevrt(tree, max, min);
	printf("max z: %f\nmin z: %f\n", max->z, min->z);
	if (sdev > maxsdev){
		tree->leaf = 0;
		tree->sub1 = (rtree*)malloc(sizeof(rtree));
		tree->sub2 = (rtree*)malloc(sizeof(rtree));
		/* 
		 * Create new subtrees, starting with the lowest and highest z values possible.
		 * Sub1 starts with the highest point, sub2 with the lowest.
		 */
		setxyz(&tree->sub1->p1, max->x, max->y, max->z);
		setxyz(&tree->sub1->p2, max->x, max->y, max->z);
		setxyz(&tree->sub2->p1, min->x, min->y, min->z);
		setxyz(&tree->sub2->p2, min->x, min->y, min->z);
		for (i = 0; i < tree->n; i++){
			/* 
			 * putrt(tree, &tree->points[i]);
			 *
			 * Use this once putrt is fully written
			 */
			//		printf("Z - %f\n", tree->points[i].z);
			sum1 = rszsum(tree->sub1, &tree->points[i]);
			sum2 = rszsum(tree->sub2, &tree->points[i]);
			if (sum2 > sum1){
				putrt(tree->sub1, &tree->points[i]);
			} else {
				putrt(tree->sub2, &tree->points[i]);
			}
		}
		/* Free any unneeded memory in the subtrees, then free the original point array */
		free(tree->points);
		tree->points = NULL;
		tree->n = 0;
		return 1;
	}	
	return 0;
}

/* Recursively resize the tree, return false if rebuilding might be necessary */
int resizert(rtree * tree){
	return 0;
}

/* Recursively rebuild the entire tree, optimizing search time */
void rebuildrt(rtree * tree) {

}
