#include <stdio.h>
#include <stdlib.h>
#include "rtree.h"
#include "qdbmp.h"
#include "hmap.h"

#define TESTFILE "testing/checker.bmp"

int main(int argc, char *argv[]) {
	int i;
	int pcount; /* Number of pixels/points in bitmap */
	rtree *rrt, *grt, *brt; /* R, G, B channels */
	point* p;

	rrt = defaultrt();
	grt = defaultrt();
	brt = defaultrt();

	for(i = 0; i < argc; i++) {
		printf("argument at [%d]: %s\n", i, argv[i]);
	}
	if (argc > 1){
		printf("Loading BMP %s\n", argv[1]);
		if (!loadbmp(argv[1], rrt, grt, brt)){
			printf("Failed to load BMP. Exiting.\n");
			exit(0);
		}
	} else {
		printf("Using default BMP %s\n", TESTFILE);
		if (!loadbmp(TESTFILE, rrt, grt, brt)){
			printf("Failed to load BMP. Exiting.\n");
			exit(0);
		}
	}
	if (!(rrt && grt && brt)){
		printf("R/G/B trees do not all exist. Exiting.\n");
		exit(0);
	}

	p = (point*)malloc(sizeof(point));
	setxyz(p, 7, 1, 255);
	rtree* stree = pfindrt(rrt, p);
	if (stree){
		printf("Found point %s in an rtree containing %d points.\n", tostringp(p), stree->pa.len);
	} else {
		printf("Did not find point %s in the tree.\n", tostringp(p));
	}

	tostringrt(rrt);
 	rect* r1 = malloc(sizeof(rect));
 	setxyz(&r1->p1, 0, 0, 0);
 	setxyz(&r1->p2, 1, 1, 1);

 	point* p1 = malloc(sizeof(point));
 	//inside
 	setxyz(p1, 0, 0, 0);

 	rect* r2 = malloc(sizeof(rect));
 	setxyz(&r2->p2, .5, .5, .5);
 	setxyz(&r2->p1, 2, 2, 2);

	point* px1 = malloc(sizeof(point));
	setxyz(px1, 0, 0, 0);

	parray* pa = malloc(sizeof(parray));
	addpa(pa, px1);
	printf("lenpa: %d\n", (int) lenpa(pa));

	point* px2 = malloc(sizeof(point));
	setxyz(px2, 10, 10, 10);

	parray* pa2 = malloc(sizeof(parray));

	addpa(pa2, px2);
	printf("lenpa2: %d\n", (int) lenpa(pa2));

	parray* pax = mergepa(pa, pa2);
	printf("lenpax: %d\n", (int) lenpa(pax));

	tostringpa(pax);

	free(pa);
	free(pa2);
	free(pax);

	rtree* rttest = (rtree*) malloc(sizeof(rtree));
	rttest->leaf = 1;

	point* prttest = (point*) malloc(sizeof(point) * 5);
	
	setxyz(&prttest[0], 0, 0, 0);
	setxyz(&prttest[1], 1, 1, 1);
	setxyz(&prttest[2], 2, 2, 2);
	setxyz(&prttest[3], 3, 3, 3);
	setxyz(&prttest[4], 4, 4, 4);
	
	bputrt(rttest, prttest, 5);
	
	rect* rtest = (rect*) malloc(sizeof(rect));
	rtest->p1 = prttest[0]; 
	rtest->p2 = prttest[3];
	
	tostringpa(rectqrt(rttest, rtest));

	return 0;
}
