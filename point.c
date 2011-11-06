/*
 * A basic 3D point.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct point{
	double x, y, z;
} point;

void setxyz(point* p, double x, double y, double z);
int peq(point* p1, point* p2);
int pinrng(double lower, double upper, double p, int inclusive);
char* tostringp(point* p);

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

/* returns a string representation of the point in standard coordinate format */
char* tostringp(point* p) {
	char* buffer = (char*) malloc(sizeof(char) * 256);
	int buflen = sprintf(buffer, "(%.2lf, %.2lf, %.2lf)", p->x, p->y, p->z);
	buffer = realloc(buffer, sizeof(char) * buflen);
	return buffer;
}
