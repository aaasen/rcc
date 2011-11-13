#include <stdio.h>
#include <stdlib.h>
#include "rtree.h"
#include "qdbmp.h"
#include "hmap.h"
#include "sphere.h"
#include "cylinder.h"

#define TESTFILE "testing/checker.bmp"
#define OUTFILE "out.bmp"

enum FLAGS {
	NONE,
	MAXSDEV,
	OUTFILENAME
} flags;

int main(int argc, char *argv[]) {
	int i;
	int pcount; /* Number of pixels/points in bitmap */
	rtree *rrt, *grt, *brt; /* R, G, B channels */
	point* p;
	char* arg, *filename, *outfilename;
	int nextmaxsdev, nextoutfile;
	enum FLAGS curflag; /* Current flag */

	rrt = creatert();
	grt = creatert();
	brt = creatert();

	filename = NULL;
	outfilename = OUTFILE;
	curflag = 
	curflag = NONE;
	
	for (i = 1; i < argc; i++){
		arg = argv[i];
		if (arg[0] == '-'){
		  switch (arg[1]){
		  case 'm':
		  	curflag = MAXSDEV;
			printf("Setting standard deviation\n");
			break;
		  case 'o':
		  	curflag = OUTFILENAME;
			break;
		  }
		  continue;
		} else {
			switch(curflag){
			case MAXSDEV:
				setmaxsdev(atoi(argv[i]));
				printf("Set maximum standard deviation to %d\n", atoi(argv[i]));
				nextmaxsdev = 0;
				break;
			case OUTFILENAME:
				outfilename = arg;
				printf("Output file set to %s\n", outfilename);
				nextoutfile = 0;
				break;
			default:
				filename = arg;
				break;
			}
			curflag = NONE;
		}
	}

	
	
	for(i = 0; i < argc; i++) {
		printf("argument at [%d]: %s\n", i, argv[i]);
	}
	if (filename){
		printf("Loading BMP %s\n", filename);
		if (!loadbmp(filename, rrt, grt, brt)){
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
	savebmp(outfilename, rrt, grt, brt);
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
}
