#include <stdio.h>
#include <stdlib.h>
#include "rtree.h"

int main(void){
  rtree* rt = (rtree*)malloc(sizeof(rtree));
  rt->points = (point*)malloc(sizeof(point) * 2);
  rt->leaf = 1;
  rt->n = 2;
  rt->points[1].z = 45;
  rt->points[0].z = 40;
  subrt(rt);
  printf("Standard Deviation: %f\n", sdevrt(rt, NULL, NULL));
  return 0;
}
