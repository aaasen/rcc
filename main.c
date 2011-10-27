#include <stdio.h>
#include <stdlib.h>
#include "rtree.h"

int main(int argc, char *argv[]) {
	int i; for(i = 0; i < argc; i++) {
		printf("argument at [%d]: %s\n", i, argv[i]);
	}
  	point* points = (point*)malloc(sizeof(point) * 4);
	rtree* rt = (rtree*)malloc(sizeof(rtree));
	rt->leaf = 1;
	points[1].z = 1;
	points[0].z = 100;
	points[2].z = 60;
	points[3].z = 70;
	printf("First Point's z: %.2f\n", points[0].z);
	bputrt(rt, points, 4);
	subrt(rt);
	printf("Standard Deviation: %f\n", sdevrt(rt, NULL, NULL));
	if (rt->sub1){
		printf("n1: %d\nn2: %d\n", rt->sub1->n, rt->sub2->n);
		printf("Standard Deviation sub1: %f\n", sdevrt(rt->sub1, NULL, NULL));
		printf("Standard Deviation sub2: %f\n", sdevrt(rt->sub2, NULL, NULL));
	}
	point* p = (point*)malloc(sizeof(point));
	rtree* stree = pfindrt(rt, p);
	if (stree){
		printf("Found point with z = %.2f in an rtree containing %d points.\n", p->z, stree->n);
	} else {
		printf("Did not find point with z = %.2f in the tree.\n", p->z);
	}
	rect* r1 = malloc(sizeof(rect));
	point* r1p1 = malloc(sizeof(point));
	point* r1p2 = malloc(sizeof(point));
	setxyz(r1p1, 0, 1, 0);
	setxyz(r1p2, 1, 0, 1);
	r1->p1 = *r1p1;
	r1->p2 = *r1p2;
	
	point* p1 = malloc(sizeof(point));
	//inside
/*	setxyz(p1, 0, 0, 0);*/
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
	
	printf("pinrng: %d\n", pinrng(10, 20, 50, 1));
	
	rect* r2 = malloc(sizeof(rect));
	point* r2p1 = malloc(sizeof(point));
	point* r2p2 = malloc(sizeof(point));
	setxyz(r2p1, 5, 6, 5);
	setxyz(r2p2, 6, 5, 6);
	r2->p1 = *r2p1;
	r2->p2 = *r2p2;
	
	printf("rinr: %d\n", rinr(r1, r2));
	
	return 0;
}
