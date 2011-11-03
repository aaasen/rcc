#include <stdio.h>
#include <stdlib.h>
#include "rtree.h"
#include "qdbmp.h"
#include "hmap.h"

#define TESTFILE "testing/checkers.bmp"

int main(int argc, char *argv[]) {
	int i;
	int pcount; /* Number of pixels/points in bitmap */
	rtree *rrt, *grt, *brt; /* R, G, B channels */
	point* p;

	for(i = 0; i < argc; i++) {
		printf("argument at [%d]: %s\n", i, argv[i]);
	}
/*	if (argc > 1){*/
/*		loadbmp(argv[1], rrt, grt, brt);*/
/*		printf("Loaded BMP\n");*/
/*	} else {*/
/*		loadbmp(TESTFILE, rrt, grt, brt);*/
/*		printf("Using default BMP\n");*/
/*	}*/


/*	printf("Standard Deviation: %f\n", sdevrt(rrt, NULL, NULL));*/
/*	if (rrt->sub1){*/
/*		printf("n1: %zu\nn2: %zu\n", rrt->sub1->pa.len, rrt->sub2->pa.len);*/
/*		printf("Standard Deviation sub1: %f\n", sdevrt(rrt->sub1, NULL, NULL));*/
/*		printf("Standard Deviation sub2: %f\n", sdevrt(rrt->sub2, NULL, NULL));*/
/*	}*/
/*	p = (point*)malloc(sizeof(point));*/
/*	setxyz(p, 0, 0, 60);*/
/*	rtree* stree = pfindrt(rrt, p);*/
/*	if (stree){*/
/*		printf("Found point with z = %.2f in an rtree containing %zu points.\n", p->z, stree->pa.len);*/
/*	} else {*/
/*		printf("Did not find point with z = %.2f in the tree.\n", p->z);*/
/*	}*/

 	rect* r1 = malloc(sizeof(rect));
 	/* point* r1p1 = malloc(sizeof(point)); */
 	/* point* r1p2 = malloc(sizeof(point)); */
 	setxyz(&r1->p1, 0, 0, 0);
 	setxyz(&r1->p2, 1, 1, 1);
 	/* r1->p1 = *r1p1; */
 	/* r1->p2 = *r1p2; */

 	point* p1 = malloc(sizeof(point));
 	//inside
 	setxyz(p1, 0, 0, 0);
 /*	setxyz(p1, .5, .5, .5);*/
 /*	setxyz(p1, 1, 1, 1);*/
 	//outside
 /*  setxyz(p1, 100, 100, 100);*/
 /*	setxyz(p1, -100, -100, -100);*/
 /*	setxyz(p1, -.5, -.5, -.5);*/
 /*	setxyz(p1, 100, 0, 0);*/
 /*	setxyz(p1, 0, 100, 0);*/
 /*	setxyz(p1, 0, 0, 100);*/
/* 	printf("pinr: %d\n", pinr(r1, p1));*/

/* 	printf("pinrng: %d\n", pinrng(10, 20, 50, 1)); */

 	rect* r2 = malloc(sizeof(rect));
 	/* point* r2p1 = malloc(sizeof(point)); */
 	/* point* r2p2 = malloc(sizeof(point)); */
 	setxyz(&r2->p2, .5, .5, .5);
 	setxyz(&r2->p1, 2, 2, 2);
 	/* r2->p1 = *r2p1; */
 	/* r2->p2 = *r2p2; */

/* 	printf("rinr: %d\n", rinr(r1, r2));*/

    point* px1 = malloc(sizeof(point));
    setxyz(px1, 0, 0, 0);

    parray* pa = malloc(sizeof(parray));
    addpa(pa, px1);
    printf("lenpa: %d\n", (int) lenpa(pa));

/*    rempa(pa, px1);*/

    point* px2 = malloc(sizeof(point));
    setxyz(px2, 10, 10, 10);
    //printf("%s\n", tostringp(px2));

    parray* pa2 = malloc(sizeof(parray));
    //printf("lenpa2: %d\n", (int) lenpa(pa2));
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
	
	//parray* yay = rectqrt(rttest, rtest);
	
/*	rectqrt(rttest, rtest);*/
	
	tostringpa(rectqrt(rttest, rtest));

/*	freert(rrt);*/
/*	freert(grt);*/
/*	freert(brt);*/
/*  free(p);*/
/*	free(p1);*/
/*	free(r1);*/
/*	free(r2);*/

	return 0;
}
