
// ECE556 - Copyright 2011 University of Wisconsin-Madison.  All Rights Reserved.
// $HeadURL:  $
// $Id: util.cpp 152 2011-01-19 02:05:45Z shojaei $


#include "util.h"
#include <cmath>

using std::floor;

/********************************************************************************
 *    Placement/Routing conversion
 ********************************************************************************/
point3d ptog(point3d p, int llx, int lly, int tWidth, int tHeight)
{
  point3d g;
  g.x = (p.x - llx) / tWidth;
  g.y = (p.y - lly) / tHeight;
  g.z = p.z - 1;
  return g;
}

point3d gtop(point3d g, int llx, int lly, int tWidth, int tHeight)
{
  point3d p;
  p.x = g.x * tWidth + llx;
  p.y = g.y * tHeight + lly;
  p.z = g.z + 1;
  return p;
}



/********************************************************************************
 *   Edges
 ********************************************************************************/

bool operator==(edge e1, edge e2)
{
  return e1.first == e2.first && e1.second == e2.second;
}

edge makeEdge(point3d p1, point3d p2)
{
  edge e;
  e.first.x = p1.x;
  e.first.y = p1.y;
  e.first.z = p1.z;

  e.second.x = p2.x;
  e.second.y = p2.y;
  e.second.z = p2.z;
  return e;
}

string edgeToString(edge e)
{
  char buff[80];
  sprintf(buff, "(%d,%d,%d)-(%d,%d,%d)", e.first.x, e.first.y, e.first.z,
          e.second.x, e.second.y, e.second.z);
  return string(buff);
}

/********************************************************************************
 *  Points
 ********************************************************************************/
/* operator< in 3d */
bool lessThan3d(point3d p1, point3d p2)
{
  if (p1.x != p2.x)
    return p1.x < p2.x;
  else if (p1.y < p2.y)
    return p1.y < p2.y;
  else
    return p1.z < p2.z;
}

bool equal3d(point3d p1, point3d p2)
{
  return p1.x == p2.x && p1.y == p2.y && p1.z == p2.z;  
}

bool nequal3d(point3d p1, point3d p2)
{
  return !equal3d(p1, p2);
}


/* operator< - used to compare points (only considers 2d) */
bool operator<(point3d p1, point3d p2)
{
  if (p1.x != p2.x)
    return p1.x < p2.x;
  else
    return p1.y < p2.y;
}

/* operator== - used to compare points (only considers 2d) */
bool operator==(point3d p1, point3d p2)
{
  return p1.x == p2.x && p1.y == p2.y;
}

/* operator!= - used to compare points (only considers 2d) */
bool operator!=(point3d p1, point3d p2)
{
  return !(p1 == p2);
}


/********************************************************************************
 *  Miscellaneous utilities
 ********************************************************************************/
bool isVertical(edge e)
{
  return e.first.y != e.second.y;
}

bool isHorizontal(edge e)
{
  return e.first.x != e.second.x;
}

bool isVia(edge e)
{
  return !isVertical(e) & !isHorizontal(e);
}
