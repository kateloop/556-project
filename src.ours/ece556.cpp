// ECE556 - Copyright 2011 University of Wisconsin-Madison.  All Rights Reserved.
// $HeadURL:  $
// $Id: ece556.cpp 152 2011-02-18 02:19:45Z shojaei $

/*! \file ece556.cpp
    \brief source code for the ECE556 API.
    
    Details.
*/

#include "ece556.h"

int readBenchmark(const char *fileName, RoutingInst *rst){
  ifstream inf;
  inf.open(fileName, ifstream::in);

  int xGrid, yGrid, zGrid;	// Global routing grid dimensions
  vector<int> vCap, hCap;       // Default vertical and horizontal capacity for each layer 
  int llx, lly;			// Lower left {x,y} for global routing grid
  int tWidth, tHeight;		// Tile width/height of a bin
  int numNets;			// Number of nets
  int numBlockages;		// Number of blockages

  // Granularity of grid
  inf >> xGrid >> yGrid >> zGrid;

  // Vertical capacities
  for (int i = 0; i < zGrid; i++) {
    int cap;
    inf >> cap;
    vCap.push_back(cap);
  }

  // Horizontal capacities
  for (int i = 0; i < zGrid; i++) {
    int cap;
    inf >> cap;
    hCap.push_back(cap);
  }
  
  // LLX, LLY
  inf >> llx >> lly;

  // Parse nets
  inf >> numNets;
  for (int i = 0; i < numNets; i++) {
    string netName;
    int netId;
    int numPins;
    int netSize;		// Ignored
    inf >> netName >> netId >> numPins >> netSize;

    // TODO : CONSTRUCT NET

    // Parse pins
    for (int j = 0; j < numPins; j++) {
      point3d p;
      inf >> p.x >> p.y >> p.z;

      // TODO : ADD POINT TO NET 
    }
  }

  // Blockages
  inf >> numBlockages;
  for (int i = 0; i < numBlockages; i++) {
    
  }

  return 1;
}


int release(RoutingInst *rst){
  /*********** TO BE FILLED BY YOU **********/

  return 1;
}
  

