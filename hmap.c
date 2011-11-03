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
	rtree* rt;
	point* rpoint, *gpoint, *bpoint;
	point* points;
  	BMP* img;
	int w, h, pcount; /* Number of pixels/points in bitmap */
	int verify; /* Used to verify image load success */
	char* rchar, *bchar, *gchar;
	int i, j; /* Iterators */;
	
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

	/* Allocate errything */
	/* r = (rtree*)malloc(sizeof(rtree)); */
	/* g = (rtree*)malloc(sizeof(rtree)); */
	/* b = (rtree*)malloc(sizeof(rtree)); */
	/* defaultpa(&r->pa); */
	/* defaultpa(&g->pa); */
	/* defaultpa(&b->pa); */
	/* r = *defaultrt(); */
	/* g = *defaultrt(); */
	/* b = *defaultrt(); */
	rchar = (char*)malloc(sizeof(char));
	gchar = (char*)malloc(sizeof(char));
	bchar = (char*)malloc(sizeof(char));
	
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
			BMP_GetPixelRGB(img, i, j, rchar, gchar, bchar);
			//			printf("OUTPUT: %d, %d, %d\n", atoi(rchar), atoi(gchar), atoi(bchar));
			setxyz(rpoint, i, j, atoi(rchar));
			setxyz(gpoint, i, j, atoi(gchar));
			setxyz(bpoint, i, j, atoi(bchar));
			putrt(r, rpoint);
			putrt(g, gpoint);
			putrt(b, bpoint);
		}
	}

	/* Subdivide the three rtrees */
	subrt(r), subrt(g), subrt(b);

	rt = (rtree*)malloc(sizeof(rtree));
	return 1;
}
