#ifndef UTIL_H
#define UTIL_H

#include <utility>
#include <vector>
#include <stdio.h>
#include <string>

using std::vector;
using std::pair;
using std::string;

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

/*
 * Debug printing
 */
extern string edgeToString(edge e);

/**
 *  A route
 **/
typedef vector<edge> route;

/*
 *  Point comparisons
 */
bool operator<(point3d p1, point3d p2);
bool operator==(point3d p1, point3d p2);
bool operator!=(point3d p1, point3d p2);

bool lessThan3d(point3d p1, point3d p2);
bool equal3d(point3d p1, point3d p2);
bool nequal3d(point3d p1, point3d p2);

/********************************************************************************
 *  Maps
 ********************************************************************************/
class edgeComp2d {
 public: 
  bool operator()(const edge e1, const edge e2)
  {
    edge ep1, ep2;

    // Normalize points
    if (e1.first < e1.second) {
      ep1 = edge(e1.second, e1.first);
    } else {
      ep1 = e1;
    }
    if (e2.first < e2.second) {
      ep2 = edge(e2.second, e2.first);
    } else {
      ep2 = e2;
    }
    return ep1 < ep2;
  }
};

class edgeComp3d {
 public: 
  bool operator()(const edge e1, const edge e2)
  {
    edge ep1, ep2;

    // Normalize points
    if (lessThan3d(e1.first,e1.second)) {
      ep1 = edge(e1.second, e1.first);
    } else {
      ep1 = e1;
    }
    if (lessThan3d(e2.first, e2.second)) {
      ep2 = edge(e2.second, e2.first);
    } else {
      ep2 = e2;
    }
    
    // Compare
    if (nequal3d(ep1.first, ep2.first))
      return lessThan3d(ep1.first, ep2.first);
    else
      return lessThan3d(ep1.second, ep2.second);
  }
};




/********************************************************************************
 *    Placement/Routing conversion
 ********************************************************************************/
/* Placement routing grid coordinates to global routing grid */
point3d ptog(point3d p, int llx, int lly, int tWidth, int tHeight);

/* Routing grid to placement grid */
point3d gtop(point3d g, int llx, int lly, int tWidth, int tHeight);

/********************************************************************************
 *   Edges
 ********************************************************************************/
edge makeEdge(point3d p1, point3d p2);
string edgeToString(edge e);

bool isVertical(edge e);
bool isHorizontal(edge e);
bool isVia(edge e);

#endif
