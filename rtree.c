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

/* Efficiently bulk add all of the points in p */
void bputrt(rtree* tree, point* p, int n){
  tree->points = (point*)realloc(tree->points, n + tree->n);
  int i;
  for (i = 0; i < n; i ++){
    tree->points[i + tree->n] = p[i];
  }
  tree->n += n;
  subrt(tree);
}

/* Subdivide the selected rtree if a leaf and if meets subdivision reqs */
int subrt(rtree* tree){
  return 0;
}

/* Recursively find and remove the point from the tree */
rtree* remrt(rtree* tree, point * p){
  if (node){
    //Check if point is in list, and remove
  } else {
    
  }
}
