/*
 * HEIGHT MAP
 * rtree r, g, b <---> bmp || rcc
 */

#include <stdio.h>
#include <stdlib.h>
#include "rtree.h"
#include "point.h"
#include "qdbmp.h"

typedef struct{
	unsigned long mnum; /* Magic number, always 0xA9D7FABA */
	unsigned long checksum;
    unsigned long long flags;
    unsigned int channelspec;
    unsigned int xpix; /* Length of image x axis */
    unsigned int ypix; /* Length of image y axis */
    unsigned short coldepth; /* Color depth in bytes */
} RCC_HEADER;

typedef struct{
	/*
	 * TODO The color variable sizes will need to be changed to handle images
	 * with more color depth at some point, so this struct is temporary
	 */
	unsigned short r; /* Red color bitmask */
	unsigned short g;
	unsigned short b;
	unsigned short a;
	unsigned long cbsize; /* Size of channel body */
} RCC_CHANNEL_HEADER;

/* We need this because we can't save to the bitmap one color value at a time */
typedef struct{
	unsigned short r;
	unsigned short g;
	unsigned short b;
} pixel;

/* Private 2D rectangle struct with color value */
typedef struct{
	unsigned int x1; /* 1 = top left, 2 = bottom right */
	unsigned int y1;
	unsigned int x2;
	unsigned int y2;
	unsigned short color;
} crect;

int loadbmp(char* file, rtree* r, rtree* g, rtree* b);
int savebmp(char* file, rtree* r, rtree* g, rtree* b);
int savercc(char* file, rtree* r, rtree* g, rtree* b);
int rcctobmp(char* rccfile, char* bmpfile);
int rrcchead(FILE* compressed, RCC_HEADER* header);
void rchead(FILE* compressed, RCC_CHANNEL_HEADER* header);


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

/* Save the rtrees to the specified bitmap file, return true if successful. */
int savebmp(char* file, rtree* r, rtree* g, rtree* b){
	BMP* img;
	FILE* file;
	pixel* data;
	parray *rpa, *gpa, *bpa;	
	int i;
	
	img = BMP_Create(getwr(r->mbr), gethr(r->mbr), getdr(r->mbr));
	rpa = getpointsrt(r);
	gpa = getpointsrt(g);
	bpa = getpointsrt(b);
	
	for (i = 0; i < rpa->len; i++){
		BMP_SetPixelRGB(img, rpa->points[i].x, rpa->points[i].y, avgzpa(rpa->points), avgzpa(gpa->points), avgzpa(bpa->points))
	}
	
	file = fopen(file, 'r');
}


/* Convert rcc file directly to bitmap, returns true if successful */
int rcctobmp(char* rccfile, char* bmpfile){
	/* Do I really have to define all of the variables at the top? */
    FILE* compressed ;
    BMP* target;
    RCC_HEADER* head;
    unsigned int lsize; /* Size of the file's representation of an rtree leaf */
    RCC_CHANNEL_HEADER* chead; /* Channel header, reused for each channel */
    crect* leaf; /* Rectangle used for reading each node within the for loop */
    pixel* data; /* Raw data to be saved to bitmap, stored x, y */
    int i, j, k; /* Iterators */
    
    compressed = fopen(rccfile, "r");
    if (!compressed){
        return 0; /* Unable to open rcc file */
    }
    head = malloc(sizeof(RCC_HEADER));
    if (!rrcchead(compressed, head)){
    	return 0;
    }
    
    lsize = sizeof(crect); /* This will have to be derived from the header */

    /* Converts the data from the rcc into pixels */
    data = malloc(lsize*head->xpix*head->ypix);
    while (!feof(compressed)){
    	chead = malloc(sizeof(RCC_CHANNEL_HEADER));
    	rchead(compressed, chead);
    	for (i = 0; i < chead->cbsize; i += lsize){
    		fread(leaf, lsize, 1, compressed);
    		for (j = leaf->x1; j <= leaf->x2; j++){
    			for (k = leaf->y1; k <= leaf->y2; j++){
    				data[i*head->xpix + j].r+=chead->r & leaf->color;
    				data[i*head->xpix + j].g+=chead->g & leaf->color;
    				data[i*head->xpix + j].b+=chead->b & leaf->color;
    			}
    		}
    	}
      free(chead);
    }
    
    /* Save to BMP */
    target = BMP_Create(head->xpix, head->ypix, head->coldepth);
    k = 0;
    for (i = 0; i < head->xpix; i++){
    	for (j = 0; j < head->ypix; j++){
    		BMP_SetPixelRGB(target, i, j, data[k].r, data[k].g, data[k].b);
    		k++;
    	}
    }
    BMP_WriteFile(target, bmpfile);
    
    BMP_Free(target);
    return 1;
}

/* Reads the header of an rcc file, returns true if the file is valid */
int rrcchead(FILE* compressed, RCC_HEADER* header){
	fread(header, sizeof(RCC_HEADER), 1, compressed);
   	if (header->mnum != 0xA9D7FABA){
    	return 0; /* File not an rcc file */
    }
    /* TODO check the checksum */
    return 1;
}

/* Read a channel header starting at the position indicator of the given file */
void rchead(FILE* compressed, RCC_CHANNEL_HEADER* header){
	fread(header, sizeof(RCC_CHANNEL_HEADER), 1, compressed);
}
