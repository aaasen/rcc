#include <stdio.h>
#include <stdlib.h>
#include "rtree.h"

int main(void){
  rtree* rt = (rtree*)malloc(sizeof(rtree));
  rt->points = (point*)malloc(sizeof(point));
  rt->n++;
  return 0;
}
