// ECE556 - Copyright 2011 University of Wisconsin-Madison.  All Rights Reserved.
// $HeadURL:  $
// $Id: util.cpp 152 2011-01-19 02:05:45Z shojaei $


#include "util.h"
#include <cmath>

using std::floor;

point3d ptog(point3d p, int llx, int lly, int tWidth, int tHeight)
{
  point3d g;
  g.x = (p.x - llx) / tWidth;
  g.y = (p.y - lly) / tHeight;
  g.z = p.z - 1;
  return g;
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
