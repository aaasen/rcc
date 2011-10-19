#include <stdlib.h>

typedef struct point{
  double x, y, z;
} point;

typedef struct rtree{
  int n;/* Number of points in the section of a tree */
  point * points;/* Array of points within the rtree */
  double x, y, z;/* Corner of prism */
  double x1, y1, z1;/* Opposite corner of prism */
  int leaf;/* True if this tree is a leaf, false if a branch */
  /* Possibly make into an n-child rtree, if faster */
  struct rtree * sub1;/* First sub tree if a leaf node */
  struct rtree * sub2;/* Second sub tree if a leaf node */
} rtree;

/* Add the specified point to the specified rtree */
rtree* putrt(rtree * tree, point * p){
  if (tree->leaf){
    //TODO Add point to points
    //Realloc as necessary
    resize(tree);
  }
}

/* Recursively find and remove the point from the tree */
rtree* remrt(rtree * tree, point * p){
  if (node){
    //Check if point is in list, and remove
  } else {

  }
}
