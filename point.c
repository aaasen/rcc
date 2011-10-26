#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct point{
	double x, y, z;
} point;

void setxyz(point* p, double x, double y, double z);
int peq(point* p1, point* p2);

/* Give the point the specified x, y, z */
void setxyz(point* p, double x, double y, double z){
	if(p == NULL) {
		printf("NULL POINTER\n");
	}
	p->x = x;
	p->y = y;
	p->z = z;
}

/* TODO revise peq and make sure it isn't causing a segfault */
int peq(point* p1, point* p2) {
	return p1->x == p2->x && p1->y == p2-> y && p1->z == p2->z ? 1 : 0;
}
