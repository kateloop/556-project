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

bool operator<(point3d p1, point3d p2)
{
  return true;
}

