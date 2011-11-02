/*
 * HEIGHT MAP
 * rtree r, g, b <---> bmp || rcc
 */

#include <stdio.h>
#include <stdlib.h>
#include "rtree.h"
#include "point.h"
#include "qdbmp.h"

/* Load the bmp image into r, g, and b rtrees */
void loadbmp(char* file, rtree* r, rtree* g, rtree* b){
  	BMP* img;
	point* points;
	point* rpoint, *gpoint, *bpoint;
	int w, h, pcount; /* Number of pixels/points in bitmap */
	rtree* rt;
	char* rchar, *bchar, *gchar;
	
	/* Load the image */
	img = BMP_ReadFile(file);
	BMP_CHECK_ERROR(stdout, -1);
	w = BMP_GetWidth(img);
	h = BMP_GetHeight(img);
	pcount = w * h;
  	points = (point*)malloc(sizeof(point) * pcount);
	
	/* Allocate errything */
	r = (rtree*)malloc(sizeof(rtree));
	g = (rtree*)malloc(sizeof(rtree));
	b = (rtree*)malloc(sizeof(rtree));
	
	/* Make r, g and b rtrees leafs */
	r->leaf = 1;
	g->leaf = 1;
	b->leaf = 1;
	
	/* Get rgb values for all pixels, store as points */
	for (w--; w>=0; w--){
		for (h--; h>=0; h++){
			rpoint = (point*)malloc(sizeof(point));
			gpoint = (point*)malloc(sizeof(point));
			bpoint = (point*)malloc(sizeof(point));
			BMP_GetPixelRGB(img, w, h, rchar, gchar, bchar);
			setxyz(rpoint, w, h, atoi(rchar));
			setxyz(gpoint, w, h, atoi(gchar));
			setxyz(bpoint, w, h, atoi(bchar));
			putrt(r, rpoint);
			putrt(g, gpoint);
			putrt(b, bpoint);
		}
	}
	
	/* Subdivide the three rtrees */
	subrt(r), subrt(g), subrt(b);

	rt = (rtree*)malloc(sizeof(rtree));
}
