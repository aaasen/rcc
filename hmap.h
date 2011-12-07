/*
 * HEIGHT MAP
 *
 * Used for input and output from bitmap and rcc files, storing or loading
 * pixels from an rtree, with z as the r, g, or b values.
 */

#ifndef HMAP_H
#define HMAP_H

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

/* Load the bmp image into r, g, and b rtrees */
int loadbmp(char* file, rtree* r, rtree* g, rtree* b);

/* Save the rtrees to the specified bitmap file, return true if successful. */
int savebmp(char* file, rtree* r, rtree* g, rtree* b);

/* Save the rtrees to the specified rcc file, return true if successful. */
int savercc(char* file, rtree* r, rtree* g, rtree* b);

/* Converts directly from rcc to bitmap, without intermediate rtrees */
int rcctobmp(char* rccfile, char* bmpfile);

/* Reads the header of an rcc file */
int rrcchead(FILE* compressed, rcchead* header);

/* Reads a channel header from an rcc file */
void rchead(FILE* compressed, rccsubhead* header);

/* Save an individual channel to a file, returns 1 if successful, 0 otherwise*/
int schead(FILE* file, rtree* rtree, uint8_t color);

/* Saves the channel body, recursively traversing each node of the rtree, returns size of body */
int scbody(FILE* file, rtree* r);

/* Takes an rtree and saves it as a channel in an rcc file, return true if successful */
int savec(FILE* file, rtree* rt, uint8_t color);


#endif
