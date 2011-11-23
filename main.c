#include <stdio.h>
#include <stdlib.h>
#include "rtree.h"
#include "qdbmp.h"
#include "hmap.h"
#include "sphere.h"
#include "cylinder.h"
#include "rectarray.h"

#define TESTFILE "testing/checker.bmp"
#define OUTFILE "out.bmp"

/* Add an element for each flag here */
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
	char* arg, *filename, *outfilename; /* Strings for use in flags.
					       These should only be set if the corresponding
					       flags have been called */
	enum FLAGS curflag; /* Current flag */


	rrt = creatert();
	grt = creatert();
	brt = creatert();
	
	loadbmp("testing/colors.bmp", rrt, grt, brt);
	savercc("out.rcc", rrt, grt, brt);
	/* rcctobmp("out.rcc", "out.bmp"); */

	filename = NULL;
	outfilename = OUTFILE;
	curflag = NONE;
	
	/* FLAG CODE */
	for (i = 1; i < argc; i++){
		arg = argv[i];
		/* How to respond to each -* command */
		/* Usually just setting the enum curflag to the appropriate value */
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
			switch(curflag){ /* How to behave if a flag has been set */
			case MAXSDEV:
				setmaxsdev(atoi(arg));
				printf("Set maximum standard deviation to %d\n", atoi(argv[i]));
							break;
			case OUTFILENAME:
				outfilename = arg;
				printf("Output file set to %s\n", outfilename);
							break;
			default:
				filename = arg;
				break;
			}
			curflag = NONE; /* The flag has taken input, and is disabled */
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
	savercc("out.rcc", rrt, grt, brt);
	parray* rrtpa = getpointsrt(rrt);
/*	printf("----points in rrt----\n");*/
/*	tostringpa(rrtpa);*/
/*	printf("----end point in rrt----\n");*/

	/* sphere* newsphere = createsphr(0, 0, 0, 5); */
	/* printsphr(newsphere); */
	/* printf("rect %s in sphere\n", rinsphr(newsphere, createrect(4, 4, 4, 7, 7, 7)) ? "is" : "is not"); */

	rect* r1;
	rect* r2;

	r1 = createrect(1, 1, 0, 5, 5, 0);
	r2 = createrect(0, 2, 0, 8, 3, 0);

	printrect(innerrect(r1, r2));

}
