#include <stdio.h>
#include <stdlib.h>
#include "rtree.h"

int main(void){
	rtree* rt = (rtree*)malloc(sizeof(rtree));
	rt->points = (point*)malloc(sizeof(point) * 4);
	rt->leaf = 1;
	rt->n = 4;
	rt->points[1].z = 0;
	rt->points[0].z = 100;
	rt->points[2].z = 10;
	rt->points[3].z = 99;
	subrt(rt);
	printf("Standard Deviation: %f\n", sdevrt(rt, NULL, NULL));
	printf("n1: %d\nn2: %d\n", rt->sub1->n, rt->sub2->n);
	printf("Standard Deviation sub1: %f\n", sdevrt(rt->sub1, NULL, NULL));
	printf("Standard Deviation sub2: %f\n", sdevrt(rt->sub2, NULL, NULL));
	
	return 0;
}
