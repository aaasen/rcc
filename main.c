#include <stdio.h>
#include <stdlib.h>
#include "rtree.h"
#include "qdbmp.h"
#include "hmap.h"
#include "sphere.h"

#define TESTFILE "testing/checker.bmp"

int main(int argc, char *argv[]) {
	int i;
	int pcount; /* Number of pixels/points in bitmap */
	rtree *rrt, *grt, *brt; /* R, G, B channels */
	point* p;

	rrt = creatert();
	grt = creatert();
	brt = creatert();

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

	//if(rrt->leaf) printf("rtt is a leaf\n");
	tostringrt(rrt);
	parray* rrtpa = getpointsrt(rrt);
/*	printf("----points in rrt----\n");*/
/*	tostringpa(rrtpa);*/
/*	printf("----end point in rrt----\n");*/

	resizert(rrt);

	parray* testpa = createpa();
	point* testp1 = createp(0,0,0);
	point* testp2 = (point*) malloc(sizeof(point));
	setxyz(testp2, 10, 10, 10);
	point* testp3 = (point*) malloc(sizeof(point));
	setxyz(testp3, 5, 5, 5);
	
	addpa(testpa, testp1);
	addpa(testpa, testp2);
	addpa(testpa, testp3);
	
	printpa(testpa);

	printrect(findmbr(testpa));
	
	sphere* newsphere = createsphr(0, 0, 0, 5);
	printsphr(newsphere);
	printf("point %s in sphere\n", pinsphr(newsphere, testp3) ? "is" : "is not");

	return 0;
}
