/*
 * HEIGHT MAP
 * rtree r, g, b <---> bmp || rcc
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "rtree.h"
#include "point.h"
#include "qdbmp.h"
#include "parray.h"

#define		RED		1
#define 	GREEN	2
#define		BLUE	4
#define		ALPHA	8

typedef struct rcchead {
	uint32_t mnum; /* Magic number, always 0xA9D7FABA */
	uint32_t checksum;
	uint64_t flags;
	uint16_t channelspec;
	uint16_t xpix; /* Length of image x axis */
	uint16_t ypix; /* Length of image y axis */
} rcchead;

typedef struct rccsubhead {
	/*
	 * TODO The color variable sizes will need to be changed to handle images
	 * with more color depth at some point, so this struct is temporary
	 */

	 /* represents color, can be bitmasked with constant color IDs in conditional for extraction */
	 /* e.g. rccsubhead->color & RED will return true if the channel is red */
	 /* this can be repeated for up to 8 (may change later) channels */
	uint8_t color;
	uint8_t depth;
	uint32_t cbsize; /* Size of channel body */
} rccsubhead;

/* We need this because we can't save to the bitmap one color value at a time */
typedef struct pixel {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} pixel;

/* Private 2D rectangle struct with color value */
typedef struct crect {
	uint16_t x1;
	uint16_t y1;
	uint16_t x2;
	uint16_t y2;
	uint8_t color;
} crect;

int loadbmp(char* file, rtree* r, rtree* g, rtree* b);
int savebmp(char* file, rtree* r, rtree* g, rtree* b);
int savercc(char* file, rtree* r, rtree* g, rtree* b);
//int rcctobmp(char* rccfile, char* bmpfile);
int rrcchead(FILE* compressed, rcchead* header);
void rchead(FILE* compressed, rccsubhead* header);
int schead(FILE* file, rtree* rtree, uint8_t color);
int savec(FILE* file, rtree* rt, uint8_t color);

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
	printf("  image dimensions: (%d, %d)\n", w, h);
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

void rtavgpoints(rtree* rt, parray* pa){
	int x, y;
	point* p;
	double avgz;

	if (rt->leaf){
		avgz = avgzpa(&rt->pa);

		/* printf("mbr: %s, %s\n", tostringp(&rt->mbr.min), tostringp(&rt->mbr.max)); */
		for (x = 0; x <= getwr(&rt->mbr); x++){
			for (y = 0; y <= gethr(&rt->mbr); y++){
			  p = (point*)malloc(sizeof(point));
			  setxyz(p, x + rt->mbr.min.x, y + rt->mbr.min.y, avgz);
			  addpa(pa, p);
			  /* printf("POINT ADDED, z = %.2f\n", p->z); */
			}
		}

	} else {
		rtavgpoints(rt->sub1, pa);
		rtavgpoints(rt->sub2, pa);
	}
}

/* Save the rtrees to the specified bitmap file, return true if successful. */
int savebmp(char* file, rtree* r, rtree* g, rtree* b){

	BMP* img, *rimg, *gimg, *bimg;
	pixel* pixels;
	pixel* pixeltemp;/* Temporary pixel for drawing to the BMP */
	point* p;
	parray *rpa, *gpa, *bpa;
	int i, x, y;
	int w, h;

	w = getwr(&r->mbr) + 1;
	h = gethr(&r->mbr) + 1;

	img = BMP_Create(w, h, 32);
	rimg = BMP_Create(w, h, 32);
	gimg = BMP_Create(w, h, 32);
	bimg = BMP_Create(w, h, 32);

	rpa = createpa();
	gpa = createpa();
	bpa = createpa();

/*	printf("W = %d -- H = %d\n", w, h);*/

	rtavgpoints(r, rpa);
	rtavgpoints(g, gpa);
	rtavgpoints(b, bpa);

	pixels = (pixel*)malloc(sizeof(pixel) * w * h);

	/* Get all pixel values */
	for (i = 0; i < rpa->len; i++){
		p = &rpa->points[i];
		pixels[(int)p->x * h + (int)p->y].r = (int)p->z;
	}
	for (i = 0; i < gpa->len; i++){
		p = &gpa->points[i];
		pixels[(int)p->x * h + (int)p->y].g = (int)p->z;
	}
	for (i = 0; i < bpa->len; i++){
		p = &bpa->points[i];
		pixels[(int)p->x * h + (int)p->y].b = (int)p->z;
	}

	/* Draw the BMP */
	for (i = 0; i < w * h; i++){
		pixeltemp = &pixels[i];
		x = (int)(i % h);
		y = (int)(i / h);
		BMP_SetPixelRGB(img, y, x,pixeltemp->r, pixeltemp->g, pixeltemp->b);
		BMP_SetPixelRGB(rimg, y, x, pixeltemp->r, 0, 0);
		BMP_SetPixelRGB(gimg, y, x, 0, pixeltemp->g, 0);
		BMP_SetPixelRGB(bimg, y, x, 0, 0, pixeltemp->b);
	}

	BMP_WriteFile(img, file);
	BMP_WriteFile(rimg, "outr.bmp");
	BMP_WriteFile(gimg, "outg.bmp");
	BMP_WriteFile(bimg, "outb.bmp");
	BMP_CHECK_ERROR(stdout, -1);
	BMP_Free(img);
	BMP_Free(rimg);
	BMP_Free(gimg);
	BMP_Free(bimg);
	return 1;
}


/* Convert rcc file directly to bitmap, returns true if successful */
int rcctobmp(char* rccfile, char* bmpfile){
    FILE* compressed ;
    BMP* target;
    rcchead* head;
    int lsize; /* Size of the file's representation of an rtree leaf */
    rccsubhead* chead; /* Channel header, reused for each channel */
    //crect* leaf; /* Rectangle used for reading each node within the for loop  */
    pixel* data; /* Raw data to be saved to bitmap, stored x, y */
    int depth = 0;
    int i, j, k; /* Iterators */

    compressed = fopen(rccfile, "r");
    if (!compressed){
        return 0; /* Unable to open rcc file  */
    }
    head = (rcchead*) malloc(sizeof(rcchead));
    if (!rrcchead(compressed, head)){
    	return 0;
    }

	lsize = (int) sizeof(crect); /* This will have to be derived from the header eventually */

    /* Converts the data from the rcc into pixels */
    data = (pixel*) malloc(sizeof(pixel)*head->xpix*head->ypix);
    printf("allocmem: %d\n", head->xpix*head->ypix);
    while (!feof(compressed)){ /* Keeps going until the file ends, so we can have any number of channels */
    	chead = (rccsubhead*) malloc(sizeof(rccsubhead));
    	rchead(compressed, chead);
     	for (i = 0; i < chead->cbsize; i += lsize){
     		crect* leaf = (crect*) malloc(sizeof(leaf));
     		fread(leaf, lsize, 1, compressed);
     		for (j = leaf->x1; j <= leaf->x2; j++){
     			for (k = leaf->y1; k <= leaf->y2; j++){
/*     				printf("allocmemqwiyw[%d, %d]: %d\n", j, k, j*head->xpix + k);*/
					/* The bitmasks are bitwise ANDed with the color value to put the color in the proper bitmap channel */
    				switch(chead->color) {
    					case RED:
    						data[j*head->xpix + k].r=leaf->color;
    						break;
    					case GREEN:
    						data[j*head->xpix + k].g=leaf->color;
    						break;
    					case BLUE:
    						data[j*head->xpix + k].b=leaf->color;
    						break;
    				}
    			}
     		}
    	}
    	depth += chead->depth;
        free(chead);
    }
     /* Save to BMP  */
     target = BMP_Create(head->xpix, head->ypix, depth);
     k = 0;
     for (i = 0; i < head->xpix; i++){
    	for (j = 0; j < head->ypix; j++){
    		BMP_SetPixelRGB(target, i, j, data[k].r, data[k].g, data[k].b);
	   		k++;
	   	}
    }
    BMP_WriteFile(target, bmpfile);

    BMP_Free(target);
    fclose(compressed);
    return 1;
}

/* Reads the header of an rcc file, returns true if the file is valid  */
int rrcchead(FILE* compressed, rcchead* header) {
 	fread(header, sizeof(rcchead), 1, compressed);
    	if (header->mnum != 0xA9D7FABA){
     		return 0; /* File not an rcc file  */
     	}
     /* TODO check the checksum  */
     return 1;
}

/* Read a channel header starting at the position indicator of the given file */
void rchead(FILE* compressed, rccsubhead* header) {
 	fread(header, sizeof(rccsubhead), 1, compressed);
}

/* Save an individual channel to a file, returns 1 if successful, 0 otherwise */
int schead(FILE* file, rtree* rtree, uint8_t color) {
 	if (file) {
 		rccsubhead* chanhead = (rccsubhead*) malloc(sizeof(rccsubhead));
 		chanhead->color = color;
 		chanhead->cbsize = 8;
 		fwrite(chanhead, sizeof(rccsubhead), 1, file);
 		return 1;
 	} return 0;
}

/* Saves the channel body, recursively traversing each node of the rtree, returns size of body */
int scbody(FILE* file, rtree* r) {
 	uint8_t avg; /* average color, to be reused */
 	crect* rect;

 	if (file) {
 		if (r->leaf) {
       /* Construct the crect for saving from the leaf  */
 			rect = malloc(sizeof(crect));
 			rect->x1 = (uint16_t) r->mbr.min.x;
 			rect->y1 = (uint16_t) r->mbr.max.x;
 			rect->x2 = (uint16_t) r->mbr.min.y;
 			rect->x2 = (uint16_t) r->mbr.max.y;
 			rect->color = (uint8_t) avgzpa(&r->pa);
 			fwrite(rect, sizeof(crect), 1, file);
 			free(rect);
 			return sizeof(crect); /* This leads to a size of the full header when calling the function on the full rtree  */
 		} else {
 			return scbody(file, r->sub1) + scbody(file, r->sub2);
 		}
 	}
}

/* Takes an rtree and saves it as a channel in an rcc file, return true if successful */
int savec(FILE* file, rtree* rt, uint8_t color) {
	long fpos; /* Position in file */
	uint32_t bsize;

	if (file) {
	 	schead(file, rt, color);
	 	fpos = ftell(file) - 4;
	 	bsize = scbody(file, rt);
		/* Return to the location 4 bytes before the end of the channel header, add save the size of the body, */
		/* and return to the end so that the main saving function cn progress  */
	 	fseek(file, fpos, SEEK_SET);
	 	fwrite(&bsize, 4, 1, file);
	 	fseek(file, 0, SEEK_END);
	 	return 1;
	} else {
		return 0;
	}
}

/* Save the rtrees to the specified rcc file, return true if successful.  */
int savercc(char* file, rtree* r, rtree* g, rtree* b) {
 	rcchead* head = malloc(sizeof(rcchead));
 	uint32_t bsize; /* channel body size */
 	FILE* target = fopen(file, "w");

    /* Save the file header, the checksum is currently just set to 0  */
 	head->mnum = 0xA9D7FABA;
 	head->checksum = 0;
 	head->flags = 0;
 	head->channelspec = 57344; /* First three bits of a 16 bit int */
 	head->xpix = abs(r->mbr.max.x - r->mbr.min.x);
 	head->ypix = abs(r->mbr.max.y - r->mbr.min.y);
 	fwrite(head, sizeof(rcchead), 1, target);

 	savec(target, r, RED);
 	savec(target, g, GREEN);
 	savec(target, b, BLUE);
 	free(head);
 	return 1;
}
