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

typedef struct{
	uint32_t mnum; /* Magic number, always 0xA9D7FABA */
	uint32_t checksum;
    uint64_t flags;
    uint16_t channelspec;
    uint16_t xpix; /* Length of image x axis */
    uint16_t ypix; /* Length of image y axis */
} RCC_HEADER;

typedef struct{
	/*
	 * TODO The color variable sizes will need to be changed to handle images
	 * with more color depth at some point, so this struct is temporary
	 */
	uint8_t r; /* Red color bitmask */
	uint8_t g;
	uint8_t b;
	uint8_t a;
	uint8_t depth;
	uint32_t cbsize; /* Size of channel body */
} RCC_CHANNEL_HEADER;

/* We need this because we can't save to the bitmap one color value at a time */
typedef struct{
	uint8_t r;
	uint8_t g;
	uint8_t b;
} pixel;

/* Private 2D rectangle struct with color value */
typedef struct{
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
int rrcchead(FILE* compressed, RCC_HEADER* header);
void rchead(FILE* compressed, RCC_CHANNEL_HEADER* header);
int schead(char* file, rtree* rtree, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
int scbody(FILE* file, rtree* r);
int savec(FILE* file, rtree* r);

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

void rtavgpoints(rtree* rt, parray* pa){
	int x, y;
	point* p;
	double avgz;

	if (rt->leaf){
		avgz = avgzpa(&rt->pa);

		/* printf("mbr: %s, %s\n", tostringp(&rt->mbr.p1), tostringp(&rt->mbr.p2)); */
		for (x = 0; x <= getwr(&rt->mbr); x++){
			for (y = 0; y <= gethr(&rt->mbr); y++){
			  p = (point*)malloc(sizeof(point));
			  setxyz(p, x + rt->mbr.p1.x, y + rt->mbr.p1.y, avgz);
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
	FILE* nepwfile;
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
	
	printf("W = %d -- H = %d\n", w, h);

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
	BMP_WriteFile(rimg, "outr,bmp");
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
/* int rcctobmp(char* rccfile, char* bmpfile){ */
/* 	/\* Do I really have to define all of the variables at the top? *\/ */
/*     FILE* compressed ; */
/*     BMP* target; */
/*     RCC_HEADER* head; */
/*     uint16_t lsize; /\* Size of the file's representation of an rtree leaf *\/ */
/*     RCC_CHANNEL_HEADER* chead; /\* Channel header, reused for each channel *\/ */
/*     crect* leaf; /\* Rectangle used for reading each node within the for loop *\/ */
/*     pixel* data; /\* Raw data to be saved to bitmap, stored x, y *\/ */
/*     int i, j, k; /\* Iterators *\/ */
/*     int  */
    
/*     compressed = fopen(rccfile, "r"); */
/*     if (!compressed){ */
/*         return 0; /\* Unable to open rcc file *\/ */
/*     } */
/*     head = malloc(sizeof(RCC_HEADER)); */
/*     if (!rrcchead(compressed, head)){ */
/*     	return 0; */
/*     } */
    
/*     lsize = sizeof(crect); /\* This will have to be derived from the header *\/ */

/*     /\* Converts the data from the rcc into pixels *\/ */
/*     data = malloc(lsize*head->xpix*head->ypix); */
/*     while (!feof(compressed)){ */
/*     	chead = malloc(sizeof(RCC_CHANNEL_HEADER)); */
/*     	rchead(compressed, chead); */
/*     	for (i = 0; i < chead->cbsize; i += lsize){ */
/*     		fread(leaf, lsize, 1, compressed); */
/*     		for (j = leaf->x1; j <= leaf->x2; j++){ */
/*     			for (k = leaf->y1; k <= leaf->y2; j++){ */
/*     				data[i*head->xpix + j].r+=chead->r & leaf->color; */
/*     				data[i*head->xpix + j].g+=chead->g & leaf->color; */
/*     				data[i*head->xpix + j].b+=chead->b & leaf->color; */
/*     			} */
/*     		} */
/*     	} */
/*       free(chead); */
/*     } */
    
/*     /\* Save to BMP *\/ */
/*     target = BMP_Create(head->xpix, head->ypix, head->coldepth); */
/*     k = 0; */
/*     for (i = 0; i < head->xpix; i++){ */
/*     	for (j = 0; j < head->ypix; j++){ */
/*     		BMP_SetPixelRGB(target, i, j, data[k].r, data[k].g, data[k].b); */
/*     		k++; */
/*     	} */
/*     } */
/*     BMP_WriteFile(target, bmpfile); */
    
/*     BMP_Free(target); */
/*     fclose(compressed); */
/*     return 1; */
/* } */

/* /\* Reads the header of an rcc file, returns true if the file is valid *\/ */
/* int rrcchead(FILE* compressed, RCC_HEADER* header) { */
/* 	fread(header, sizeof(RCC_HEADER), 1, compressed); */
/*    	if (header->mnum != 0xA9D7FABA){ */
/*     	return 0; /\* File not an rcc file *\/ */
/*     } */
/*     /\* TODO check the checksum *\/ */
/*     return 1; */
/* } */

/* /\* Read a channel header starting at the position indicator of the given file *\/ */
/* void rchead(FILE* compressed, RCC_CHANNEL_HEADER* header) { */
/* 	fread(header, sizeof(RCC_CHANNEL_HEADER), 1, compressed); */
/* } */

/* /\* Save an individual channel to a file, returns 1 if successful, 0 otherwise*\/ */
/* int schead(FILE* file, rtree* rtree, uint8_t r, uint8_t g, uint8_t b, uint8_t a) { */
/* 	if (file) { */
/* 		RCC_CHANNEL_HEADER* chanhead = malloc(sizeof(RCC_CHANNEL_HEADER)); */
/* 		chanhead->r = r; */
/* 		chanhead->g = g; */
/* 		chanhead->b = b; */
/* 		chanhead->a = a; */
/* 		chanhead->cbsize = 8; */
/* 		fwrite(chanhead, malloc(sizeofRCC_CHANNEL_HEADER), 1, file); */
/* 		return 1; */
/* 	} return 0; */
/* } */

/* /\* Saves the channel body, recursively traversing each node of the rtree, returns size of body *\/ */
/* int scbody(FILE* file, rtree* r) { */
/* 	uint8_t avg; /\* average color, to be reused *\/ */
/* 	crect* rect; */
	
/* 	if (file) { */
/* 		if (r->leaf) { */
/* 			rect = malloc(sizeof(crect)); */
/* 			rect->x1 = (uint16_t) r->mbr.p1.x; */
/* 			rect->y1 = (uint16_t) r->mbr.p2.x; */
/* 			rect->x2 = (uint16_t) r->mbr.p1.y; */
/* 			rect->x2 = (uint16_t) r->mbr.p2.y; */
/* 			rect->color = (uint8_t) avgzpa(&r->pa); */
/* 			frwite(rect, sizeof(crect), 1, file); */
/* 			free(rect); */
/* 			return sizeof(crect); */
/* 		} else { */
/* 			return scbody(file, r->sub1) + savecbody(file, r->sub2); */
/* 		} */
/* 	} */
/* } */

/* /\* Takes an rtree and saves it as a channel in an rcc file, return true if successful *\/ */
/* int savec(FILE* file, rtree* r) { */
/* 	schead(file, r); */
/* 	fpos = ftell(file) - 4; */
/* 	bsize = scbody(file, r); */
/* 	fseek(SEEK_SET, fpos, file); */
/* 	fwrite(&bsize, 4, 1, file); */
/* 	fseek(SEEK_END, 0, file); */
/* } */

/* /\* Save the rtrees to the specified rcc file, return true if successful. *\/ */
/* int savercc(char* file, rtree* r, rtree* g, rtree* b) { */
/* 	RCC_HEADER* head = malloc(sizeof(RCC_HEADER)); */
/* 	long fpos; /\* positon in file *\/ */
/* 	uint32_t bsize; /\* channel body size *\/ */
	
/* 	head->mnum = 0xA9D7FABA; */
/* 	head->checksum = 0;  */
/* 	head->flags = 0; */
/* 	head->channelspec = 57344; /\* First three bits of the 16 bit int *\/ */
/* 	head->xpix = abs(r->mbr.p2.x - r->mbr.p1.x); */
/* 	head->ypix = abs(r->mbr.p2.y - r->mbr.p1.y); */
/* 	fwrite(head, sizeof(RCC_HEADER), 1, file); */
/* 	savec(file, r); */
/* 	savec(file, g); */
/* 	savec(file, b); */
/* 	free(head); */
/* } */
