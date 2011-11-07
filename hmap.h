/*
 * HEIGHT MAP
 *
 * Used for input and output from bitmap and rcc files, storing or loading
 * pixels from an rtree, with z as the r, g, or b values.
 */

#include <stdio.h>
#include <stdlib.h>
#include "rtree.h"
#include "point.h"
#include "qdbmp.h"

#ifndef HMAP_H_
#define HMAP_H_

typedef struct{
	unsigned long mnum; /* Magic number, always 0xFE1A710 */
	unsigned long checksum;
    unsigned long long flags;
    unsigned int channelspec;
    unsigned int xpix; /* Length of image x axis */
    unsigned int ypix; /* Length of image y axis */
    unsigned short coldepth; /* Color depth in bytes */
 } RCC_HEADER;

typedef struct{
	/*
	 * The color variable sizes will need to be changed to handle images with
	 * more color depth at some point, so this struct is temporary
	 */
	unsigned short r; /* Red color bitmask */
	unsigned short g;
	unsigned short b;
	unsigned short a;
	unsigned long cbsize; /* Size of channel body */
} RCC_CHANNEL_HEADER;

typedef struct{
	unsigned int x;
	unsigned int y;
	unsigned int r;
	unsigned int g;
	unsigned int b;
	unsigned int a;
} pixel;
	

/* Load the bmp image into r, g, and b rtrees */
int loadbmp(char* file, rtree* r, rtree* g, rtree* b);

/* Save the rtrees to the specified bitmap file, return true if successful. */
int savebmp(char* file, rtree* r, rtree* g, rtree* b);

/* Save the rtrees to the specified rcc file, return true if successful. */
int savercc(char* file, rtree* r, rtree* g, rtree* b);

/* Converts directly from rcc to bitmap, without intermediate rtrees */
int rcctobmp(char* rccfile, char* bmpfile);

/* Reads the header of an rcc file */
int rrcchead(FILE* compressed, RCC_HEADER* header);

/* Reads a channel header from an rcc file */
void rchead(FILE* compressed, RCC_CHANNEL_HEADER* header);


#endif
