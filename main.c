#include <stdio.h>
#include <stdlib.h>
#include "rtree.h"
#include "qdbmp.h"
#include "hmap.h"
#include "sphere.h"
#include "cylinder.h"

#define TESTFILE "testing/checker.bmp"
#define OUTFILE "out.bmp"

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
	//tostringrt(brt);
	savebmp(OUTFILE, rrt, grt, brt);
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

	
/* Testing code to put in main */
	rect* r1 = createrect(.1, .1, 0, 1.9, 1.9, 1);
	rect* r2 = createrect(.1, .1, 0, .2, .2, 1);
	point* p1 = createp(1.0, 1.0, 1.0);
	point* p2 = createp(10.0, 10.0, 10.0);
	cylinder* cyl = createcyl(1.0, 1.0, 0.0, 2.0, 1.0);
	printf("1:[%d],[%d], 0:[%d],[%d]\n", pincyl(cyl, p1), rincyl(cyl, r1), pincyl(cyl, p2), rincyl(cyl, r2));

	return 0;
}
