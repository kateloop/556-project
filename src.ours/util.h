#ifndef UTIL_H
#define UTIL_H

#include <utility>

using std::pair;

 /**
  * A structure to represent a 3D Point. 
  */
 typedef struct
 {
   int x ; /* The x coordinate (>=0 in the routing grid)*/
   int y ; /* The y coordinate (>=0 in the routing grid)*/
   int z ; /* The z coordinate (>=0 in the routing grid)*/

 } point3d ;

/**
 * A structure to represent an edge
 */
typedef pair<point3d, point3d> edge;

bool operator<(point3d p1, point3d p2);

class edgeComp {
 public: 
  bool operator()(const edge e1, const edge e2)
  {
    return e1 < e2;
  }
};


/* Placement routing grid coordinates to global routing grid */
point3d ptog(point3d p, int llx, int lly, int tWidth, int tHeight);



#endif
