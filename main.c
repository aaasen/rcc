#include <stdio.h>
#include <stdlib.h>
#include "rtree.h"

int main(int argc, char *argv[]) {
	int i; for(i = 0; i < argc; i++) {
		printf("argument at [%d]: %s\n", i, argv[i]);
	}
	rtree* rt = (rtree*)malloc(sizeof(rtree));
	rt->points = (point*)malloc(sizeof(point) * 4);
	rt->leaf = 1;
	rt->n = 4;
	rt->points[1].z = 0;
	rt->points[0].z = 100;
	rt->points[2].z = 10;
	rt->points[3].z = 70;
	subrt(rt);
	printf("Standard Deviation: %f\n", sdevrt(rt, NULL, NULL));
	printf("n1: %d\nn2: %d\n", rt->sub1->n, rt->sub2->n);
	printf("Standard Deviation sub1: %f\n", sdevrt(rt->sub1, NULL, NULL));
	printf("Standard Deviation sub2: %f\n", sdevrt(rt->sub2, NULL, NULL));
	
	rect* r1 = malloc(sizeof(rect));
	point* r1p1 = malloc(sizeof(point));
	point* r1p2 = malloc(sizeof(point));
	setxyz(r1p1, 0, 1, 0);
	setxyz(r1p2, 1, 0, 1);
	r1->p1 = *r1p1;
	r1->p2 = *r1p2;
	
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
	
	return 0;
}
