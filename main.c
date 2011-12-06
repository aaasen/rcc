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
#define MANFILE	"manual.txt"

/* Add an element for each flag here */
enum FLAGS {
	NONE,
	MAXSDEV,
	OUTFILENAME
} FLAGS;

int main(int argc, char *argv[]) {
	int i;
	int pcount; /* Number of pixels/points in bitmap */
	rtree *rrt, *grt, *brt; /* R, G, B channels */
	point* p;
	char* arg, *filename, *outfilename; /* Strings for use in flags.
					       These should only be set if the corresponding
					       flags have been called */
	enum FLAGS curflag; /* Current flag */
	FILE* manual;
	int complevel = 5;

	rrt = creatert();
	grt = creatert();
	brt = creatert();
	
/*	rcctobmp("out.rcc", "out.bmp"); */

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
			break;
		  case 'o':
		  	curflag = OUTFILENAME;
			break;
		  case 'h':
		  	manual = fopen(MANFILE, "r");
			while(!feof(manual)) {
				printf("%c", (char) fgetc(manual));
			}
			fclose(manual);
		  	exit(EXIT_SUCCESS);
		  }
		  continue;
		} else {
			switch(curflag){ /* How to behave if a flag has been set */
			case MAXSDEV:
				complevel = atoi(arg);
				printf("Set compression level to %d\n", atoi(argv[i]));
				break;
			case OUTFILENAME:
				outfilename = arg;
/*				printf("Output file set to %s\n", outfilename);*/
				break;
			default:
				filename = arg;
/*				printf("Looking for something? Run with '-h' for documentation.\n");*/
				break;
			}
			curflag = NONE; /* The flag has taken input, and is disabled */
		}
	}

	setmaxsdev(atoi(arg));

	if (filename){
		printf("Loading rtrees from \'%s\'\n", filename);
		if (!loadbmp(filename, rrt, grt, brt)){
			printf("Failed to load BMP. Exiting.\n");
			exit(0);
		}
		printf("Saving rtrees to \'%s\' with compression level of %d\n", outfilename, complevel);
		savebmp(outfilename, rrt, grt, brt);
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
}
