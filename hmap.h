/*
 * HEIGHT MAP
 *
 * Used for input and output from bitmap and rcc files, storing or loading pixels
 * from an rtree, with z as the r, g, or b values.
 */

#include "rtree.h"

#ifndef HMAP_H_
#define HMAP_H_

void loadbmp(char* file, rtree* r, rtree* g, rtree* b); /* Load the bmp image into r, g, and b rtrees */

int savebmp(char* file, rtree* r, rtree* g, rtree* b); /* Save the rtrees to the specified bitmap file, return true if successful. */

void loadrcc(char* file, rtree* r, rtree* g, rtree* b); /* Load the rcc file into the rtrees */

int savercc(char* file, rtree* r, rtree* g, rtree* b); /* Save the rtrees to the specified rcc file, return true if successful. */

#endif
