#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct point{
	double x, y, z;
} point;

void setxyz(point* p, double x, double y, double z);
int peq(point* p1, point* p2);
int pinrng(double lower, double upper, double p, int inclusive);

/* Give the point the specified x, y, z */
void setxyz(point* p, double x, double y, double z){
	if(p == NULL) {
		printf("NULL POINTER\n");
	}
	p->x = x;
	p->y = y;
	p->z = z;
}

int peq(point* p1, point* p2) {
	return p1->x == p2->x && p1->y == p2-> y && p1->z == p2->z ? 1 : 0;
}

/* returns true if the value is in between the specified values */
/* if inclusive is true <= will be used; < will be used if not */
int pinrng(double lower, double upper, double p, int inclusive) {
	return inclusive ? (lower <= p) && (p <= upper) : (lower < p) && (p < upper);
}
