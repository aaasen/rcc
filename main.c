#include <stdio.h>
#include <stdlib.h>
#include "rtree.h"

int main(void){
  rtree* rt = (rtree*)malloc(sizeof(rtree));
  rt->points = (point*)malloc(sizeof(point) * 2);
  rt->leaf = 1;
  rt->n = 2;
  rt->points[1].z = 45;
  rt->points[0].z = 44;
  printf("Standard Deviation: %f\n", sdevrt(rt));
  return 0;
}
