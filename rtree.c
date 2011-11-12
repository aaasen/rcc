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
#include <string.h>
#include "point.h"
#include "rect.h"
#include "parray.h"
#include "sphere.h"

static int maxsdev = 5; /* Maximum standard deviation within each rtree */

typedef struct rtree{
	parray pa; /* array of points in the rtree */

	rect mbr; /* Minimum Bounding Rectangle of the node */

	int leaf;/* True if this tree is a leaf, false if a branch */
	/* Possibly make into an n-child rtree, if faster */
	struct rtree * sub1;/* First sub tree if a leaf node */
	struct rtree * sub2;/* Second sub tree if a leaf node */
} rtree;

rtree* putrt(rtree * tree, point * p);
void bputrt(rtree* tree, parray* pa);
rtree* remrt(rtree * tree, point * p);
double sdevrt(rtree * tree, point * max, point * min);
int subrt(rtree * tree);
int resizert(rtree * tree);
void rebuildrt(rtree * tree);
rtree* pfindrt(rtree* tree, point * p);
parray* searchrt(rtree* tree, void* qshape, int (pinshape)(void*, point*), int (rinshape)(void*, rect*));
parray* rsearchrt(rtree* tree, rect* qbox);
parray* sphrsearchrt(rtree* tree, sphere* qrysphere);
parray* getpointsrt(rtree* tree);
rtree* creatert();
void printrt(rtree* tree);
void freert(rtree* tree);
void setmaxsdev(int newval);

/* Add the specified point to the specified rtree
 * Can not resize the rtree, only expand it. e.g. a large prism has been predefined
 * so that all points that fall within it will be put in it. After a single point
 * is inserted, the tree should NOT be shrunk to only fit that point.
 */
rtree* putrt(rtree * tree, point * p){
	if (!(tree->sub1 || tree->sub2)){
		tree->leaf = 1;
	}
	if (p){
		if (tree->leaf){
			addpa(&tree->pa, p);
		} else {
			/* Select the subtree that can contain the point with the least expansion, or */
			/* if both require the same expansion, add to the first. */

			double rszsub1 = rszsum(&tree->sub1->mbr, p);
			double rszsub2 = rszsum(&tree->sub2->mbr, p);
			rtree* bestnode = rszsub1 >= rszsub2 ? tree->sub1 : tree->sub2;
			putrt(bestnode, p);
			
		}
	} else {
	  perror("[Error]: putrt() passed a null point pointer");
	}
}

/* Efficiently bulk add all of the points in pa. */
/* Fails if tree is not a leaf. */
void bputrt(rtree* tree, parray* pa) {
	int i;
	if (tree->leaf){
		tree->pa.points = (point*)realloc(tree->pa.points, (pa->len + tree->pa.len) * sizeof(point));
		memcpy(&tree->pa.points[tree->pa.len], pa->points, pa->len * sizeof(point));
		tree->pa.len += pa->len;
		printf("%d points in tree", (int) tree->pa.len);
		subrt(tree);
		resizert(tree);
	} else {
	  perror("Warning: Did not insert points into non-leaf rtree");
	}
}

/* Recursively find and remove the point from the tree */
/* does not resize or delete the node after deletion */
rtree* remrt(rtree* tree, point * p){
	int i;

	if (!(tree->sub1 || tree->sub2)){
		tree->leaf = 1;
	}
	if (p) {
		if (tree->leaf){
			for(i = 0; i < tree->pa.len; i++) {
				if(peq(p, &tree->pa.points[i])) {
					rempa(&tree->pa, p);
				}
			}
		} else {
			remrt(tree->sub1, p);
			remrt(tree->sub2, p);
		}
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

	if (tree->pa.len <= 1 || !(tree->pa.points) || !(tree->leaf)){
		return 0;
	}
	total = 0;
	maxp = &(tree->pa.points[0]);/* Keep track of maximum z value point */
	minp = &(tree->pa.points[0]);/* Keep track of minimum z value point */
	for (i = 0; i < tree->pa.len; i++){
		total += tree->pa.points[i].z; /* THIS LINE'Y'LINE IS THE SEG'Y'SEG */
		if (tree->pa.points[i].z > maxp->z){
			maxp = &(tree->pa.points[i]);
		}
		if (tree->pa.points[i].z < minp->z){
			minp = &(tree->pa.points[i]);
		}
	}
	if (max && min){
		/* Transfer x,y,z for max and min points */
		/* free(max); */
		/* free(min); */
		setxyz(max, maxp->x, maxp->y, maxp->z);
		setxyz(min, minp->x, minp->y, minp->z);
	}
	mean = total / tree->pa.len;
	sumsqr = 0;
	for (i = 0; i < tree->pa.len; i++){
	  sumsqr += pow(tree->pa.points[i].z - mean, 2);
	}
	return sqrt(sumsqr / (tree->pa.len - 1));
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
	
	if (!tree){
		return;
	}

	resizert(tree);

	if (tree->leaf){
		max = (point*)malloc(sizeof(point));
		min = (point*)malloc(sizeof(point));
		sdev = sdevrt(tree, max, min);
		/*	printf("max z: %f\nmin z: %f\n", max->z, min->z); */

		if (sdev > maxsdev){
			tree->leaf = 0;
			/* tree->sub1 = (rtree*)malloc(sizeof(rtree)); */
			/* tree->sub2 = (rtree*)malloc(sizeof(rtree)); */
			tree->sub1 = creatert();
			tree->sub2 = creatert();
			/*
			 * Create new subtrees, starting with the lowest and highest z values possible.p
			 * Sub1 starts with the highest point, sub2 with the lowest.
			 */
			if (max){
				setxyz(&tree->sub1->mbr.p1, max->x, max->y, max->z);
				setxyz(&tree->sub1->mbr.p2, max->x, max->y, max->z);
			}
			if (min){
				setxyz(&tree->sub2->mbr.p1, min->x, min->y, min->z);
				setxyz(&tree->sub2->mbr.p2, min->x, min->y, min->z);
			}
			for (i = 0; i < tree->pa.len; i++){

				putrt(tree, &tree->pa.points[i]);
				/*			sum1 = rszsum(tree->sub1, &tree->pa.points[i]);
							sum2 = rszsum(tree->sub2, &tree->pa.points[i]);
							if (sum2 > sum1){
							putrt(tree->sub1, &tree->pa.points[i]);
							} else {
							putrt(tree->sub2, &tree->pa.points[i]);
							}*/
			}
			/* Free any unneeded memory in the subtrees, then free the original point array */
			free(tree->pa.points);
			tree->pa.points = NULL;
			tree->pa.len = 0;
			/* tree->pa.len = 0; */
			subrt(tree->sub1);
			subrt(tree->sub2);
			free(max);
			free(min);
			return 1;
		}
		free(max);
		free(min);
	} else {
		subrt(tree->sub1);
		subrt(tree->sub2);
	}
	return 0;
}

/* Recursively resize the tree, return false if rebuilding might be necessary */
/* TODO: detect if rebuild is necessary */
int resizert(rtree* tree){
	if(tree) {
		if(!(tree->sub1 || tree->sub2)) {
			tree->leaf = 1;
		}
		
		if(tree->leaf) {
			tree->mbr = *findmbr(&tree->pa);
		} else {
			tree->mbr = *findmbr(getpointsrt(tree));
			resizert(tree->sub1);
			resizert(tree->sub2);
		}
	} return 1;
}

/* Recursively rebuild the entire tree, optimizing search time */
void rebuildrt(rtree * tree) {
	parray* allpoints = getpointsrt(tree);
	
	freert(tree->sub1);
	freert(tree->sub2);
	
	bputrt(tree, allpoints);
}

/*
 * SINGLE POINT SEARCH
 * Recursively search through the rtree to find the rtree containing the
 * specified point. Return null if the point is not in the tree.
 * Assumes the rtree is properly resized.
 */
rtree* pfindrt(rtree* tree, point * p){
	int i;
	rtree* tree1;
	rtree* tree2;

	if (tree && p){
		if (tree->leaf){
			for (i = 0; i < tree->pa.len; i++){
				if (peq(&tree->pa.points[i], p)){
					return tree;
				}
			}
		} else {
			tree1 = pfindrt(tree->sub1, p);
			if (tree1){
				return tree1;
			}
			tree2 = pfindrt(tree->sub2, p);
			if (tree2){
				return tree2;
			}
		}
	} else {
		perror("[Error]: pfindrt sent a null pointer");
	}
	return NULL;/* Returned if error occurs, or if point is not found */
}

/* searchrt() is an extensible search function for rtrees */
/* it returns an array of points which are contained in qshape (which can be any, preferably geometric, struct) */
/* searchrt() requires two function pointers: */
/* 		-pinshape(), or point in shape, takes a struct which is the same type as qshape and a point* as arguments */
/*			and returns 1 if the point is in the shape and anything else if it is not */
/*		-rinshape(), or rectangle in shape, is identical in behavior to pinshape() */
/*			but takes a rect* instead of a point* as its second argument */
/* using these functions searchrt() recursively traverses the rtree and compiles an array of points */
/* which are enclosed by the given shape */
parray* searchrt(rtree* tree, void* qshape, int (pinshape)(void*, point*), int (rinshape)(void*, rect*)) {
	int i;
	parray* inquery; //parray that contains all points in the query box
	inquery = createpa();

	if (!tree->sub1 && !tree->sub2) { //logic check
		tree->leaf = 1;
	}
	if (tree && qshape) { //make sure arguments aren't null
		if(tree->leaf) { //if leaf iterate through and add all points that are in the qshape to inquery
			for(i = 0; i < tree->pa.len; i++) {
				if(pinshape(qshape, &tree->pa.points[i])) {
					addpa(inquery, &tree->pa.points[i]);
				}
			}
			printf("leaf node\n");
		} else { //if not leaf recurse through child nodes and return sum of that
			parray* sub1buf = NULL;
			parray* sub2buf = NULL;
			if(tree->sub1 && rinshape(qshape, &tree->sub1->mbr)) {
				sub1buf = searchrt(tree->sub1, qshape, pinshape, rinshape);
			}
			if(tree->sub2 && rinshape(qshape, &tree->sub2->mbr)) {
				sub2buf = searchrt(tree->sub2, qshape, pinshape, rinshape);
			}
			inquery = mergepa(sub1buf, sub2buf);
		}
		return inquery;
	}
	return NULL; //if program flows to here some arguments are null
	printf("searchrt (or relative) passed null arguments\n");
}

/* returns an array of points which are in the query rectangle */
parray* rsearchrt(rtree* tree, rect* qryrect) {
	return searchrt(tree, qryrect, (void*) pinr, (void*) rinr);
}

/* returns an array of points which are in the query sphere */
parray* sphrsearchrt(rtree* tree, sphere* qrysphere) {
	return searchrt(tree, qrysphere, (void*) pinsphr, (void*) rinsphr);
}

/* recursively builds a parray containing the points in the given node and in all of its children */
parray* getpointsrt(rtree* tree) {
	if(tree) {
		return tree->leaf ? &tree->pa : mergepa(getpointsrt(tree->sub1), getpointsrt(tree->sub2));
	}
	return NULL;
}

/* Allocate and assign everything necessary for a new leaf rtree */
rtree* creatert() {
	rtree* tree;
	
	tree = (rtree*) malloc(sizeof(rtree));
	tree->leaf = 1;
	tree->pa = *createpa();
	setxyz(&tree->mbr.p1, 0, 0, 0);
	setxyz(&tree->mbr.p2, 0, 0, 0);
	return tree;
}

/* Recursively print all points in the rtree */
void printrt(rtree* tree) {
	if (tree->leaf) {
		printpa(&tree->pa);
	} else {
		printf("{\n");
		printrt(tree->sub1);
		printf("----------\n");
		printrt(tree->sub2);
		printf("}\n");
	}
}

/* Recursively free the rtree and all of its nodes */
void freert(rtree* tree) {
	if(tree) {
		if (tree->leaf){
			free(tree->pa.points);
		} else {
			freert(tree->sub1);
			freert(tree->sub2);
		}
		free(tree);
	}
}

void tostringrt(rtree* tree){

	if (tree->leaf){
		printpa(&tree->pa);
	} else {
		printf("{\n");
		tostringrt(tree->sub1);
		printf("----------\n");
		tostringrt(tree->sub2);
		printf("}\n");
  
	}
}

void setmaxsdev(int newval){
	maxsdev = newval;
}
