/*
 * HEIGHT MAP
 * rtree r, g, b <---> bmp || rcc
 */

#include <stdio.h>
#include <stdlib.h>
#include "rtree.h"
#include "point.h"
#include "qdbmp.h"

/* Load the bmp image into r, g, and b rtrees. Return true if successful */
int loadbmp(char* file, rtree* r, rtree* g, rtree* b){
	point* rpoint, *gpoint, *bpoint;
	point* points;
  	BMP* img;
	int w, h, pcount; /* Number of pixels/points in bitmap */
	int verify; /* Used to verify image load success */
	UCHAR rchar, bchar, gchar;
	int i, j; /* Iterators */;
	int rval, gval, bval;
	
	/* Load the image */
	img = BMP_ReadFile(file);
	if (!img){
		return 0;
	}

	BMP_CHECK_ERROR( stdout, -1 );
	w = BMP_GetWidth(img);
	h = BMP_GetHeight(img);
	printf("IMG W = %d & H = %d\n", w, h);
	pcount = w * h;
  	points = (point*)malloc(sizeof(point) * pcount);

	/* Make r, g and b rtrees leafs */
	r->leaf = 1;
	g->leaf = 1;
	b->leaf = 1;
	
	/* Get rgb values for all pixels, store as points */
	for (i = w - 1; i>=0; i--){
		for (j = h - 1; j>=0; j--){
			rpoint = (point*)malloc(sizeof(point));
			gpoint = (point*)malloc(sizeof(point));
			bpoint = (point*)malloc(sizeof(point));
			BMP_GetPixelRGB(img, i, j, &rchar, &gchar, &bchar);
			rval = (int)(rchar);
			gval = (int)(gchar);
			bval = (int)(bchar);
			setxyz(rpoint, i, j, rchar);
			setxyz(gpoint, i, j, gchar);
			setxyz(bpoint, i, j, bchar);
			putrt(r, rpoint);
			putrt(g, gpoint);
			putrt(b, bpoint);
		}
	}

	/* Subdivide the three rtrees */
	subrt(r), subrt(g), subrt(b);

	BMP_Free(img);
	return 1;
}
