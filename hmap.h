/*
 * HEIGHT MAP
 *
 * Used for input and output from bitmap and rcc files, storing or loading
 * pixels from an rtree, with z as the r, g, or b values.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rtree.h"
#include "point.h"
#include "qdbmp.h"

#ifndef HMAP_H_
#define HMAP_H_

typedef struct{
	uint32_t mnum; /* Magic number, always 0xA9D7FABA */
	uint32_t checksum;
    uint64_t flags;
    uint16_t channelspec;
    uint16_t xpix; /* Length of image x axis */
    uint16_t ypix; /* Length of image y axis */
    uint8_t coldepth; /* Color depth in bytes */
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
	uint32_t cbsize; /* Size of channel body */
} RCC_CHANNEL_HEADER;

/* We need this because we can't save to the bitmap one color value at a time */
typedef struct{
	uint8_t r;
	uint8_t g;
	uint8_t b;
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

/* Save an individual channel to a file, returns 1 if successful, 0 otherwise*/
int saverccchannel(char* file, rtree* r);


#endif
