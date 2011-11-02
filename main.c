#include <stdio.h>
#include <stdlib.h>
#include "rtree.h"
#include "qdbmp.h"
#include "parray.h"

int pcount = 4;

int main(int argc, char *argv[]) {
	int i;
/*	BMP* img;*/


	for(i = 0; i < argc; i++) {
		printf("argument at [%d]: %s\n", i, argv[i]);
	}

/*	point* p1 = malloc(sizeof(point));*/
/*	setxyz(p1, 0, 0, 0);*/

/*	printf("lenpa: 0\n");*/
/*	parray* pa = malloc(sizeof(parray));*/
/*	addpa(pa, p1);*/
/*	printf("lenpa: %d\n", lenpa(pa));*/
/*	*/
/*	rempa(pa, p1);*/
/*	printf("lenpa: %d\n", lenpa(pa));*/

/*	free(p1);*/
/*	free(pa);*/

/*	if (argc >= 1){*/
/*		img = BMP_ReadFile(argv[1]);*/
/*	}*/
/*	BMP_CHECK_ERROR(stdout, -1);*/
/*  	*/
  	point* points = (point*)malloc(sizeof(point) * 4); //BMP_GetHeight(img) * BMP_GetWidth(img)
	printf("GOT HERE");
	rtree* rt = (rtree*)malloc(sizeof(rtree));
	rt->leaf = 1;
	
	points[1].z = 1;
	points[0].z = 100;
	points[2].z = 50;
	points[3].z = 70;
	bputrt(rt, points, pcount);
	printf("First Point's z: %.2f\n", points[0].z);
	subrt(rt);
	printf("Standard Deviation: %f\n", sdevrt(rt, NULL, NULL));
	if (rt->sub1){
		printf("n1: %d\nn2: %d\n", rt->sub1->n, rt->sub2->n);
		printf("Standard Deviation sub1: %f\n", sdevrt(rt->sub1, NULL, NULL));
		printf("Standard Deviation sub2: %f\n", sdevrt(rt->sub2, NULL, NULL));
	}
	point* p = (point*)malloc(sizeof(point));
	setxyz(p, 0, 0, 60);
	rtree* stree = pfindrt(rt, p);
	if (stree){
		printf("Found point with z = %.2f in an rtree containing %d points.\n", p->z, stree->n);
	} else {
		printf("Did not find point with z = %.2f in the tree.\n", p->z);
	}

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
 	printf("pinr: %d\n", pinr(r1, p1)); 

/* 	printf("pinrng: %d\n", pinrng(10, 20, 50, 1)); */

 	rect* r2 = malloc(sizeof(rect)); 
 	/* point* r2p1 = malloc(sizeof(point)); */ 
 	/* point* r2p2 = malloc(sizeof(point)); */ 
 	setxyz(&r2->p2, .5, .5, .5); 
 	setxyz(&r2->p1, 2, 2, 2); 
 	/* r2->p1 = *r2p1; */ 
 	/* r2->p2 = *r2p2; */ 

 	printf("rinr: %d\n", rinr(r1, r2)); 

	free(points);
	freert(rt);
	free(p); 
	free(p1); 
	free(r1); 
	free(r2); 

	return 0;
}
