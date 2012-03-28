// ECE556 - Copyright 2011 University of Wisconsin-Madison.  All Rights Reserved.
// $HeadURL:  $
// $Id: ece556.cpp 152 2011-02-18 02:19:45Z shojaei $

/*! \file ece556.cpp
    \brief source code for the ECE556 API.
    
    Details.
*/

#include "ece556.h"

RoutingInst* readBenchmark(const char *fileName) {
  ifstream inf;
  inf.open(fileName, ifstream::in);

  RoutingInst *rst;
  int xGrid, yGrid, zGrid;	// Global routing grid dimensions
  vector<int> *vCap, *hCap;       // Default vertical and horizontal capacity for each layer 
  int llx, lly;			// Lower left {x,y} for global routing grid
  int tWidth, tHeight;		// Tile width/height of a bin
  int numNets;			// Number of nets
  int numBlockages;		// Number of blockages
  string dummy;			// Dummy string for parsing input

  // Granularity of grid
  inf >> dummy >> xGrid >> yGrid >> zGrid;

  // Vertical capacities
  inf >> dummy >> dummy;
  vCap = new vector<int>();
  for (int i = 0; i < zGrid; i++) {
    int cap;
    inf >> cap;
    vCap->push_back(cap);
  }

  // Horizontal capacities
  inf >> dummy >> dummy;
  hCap = new vector<int>();
  for (int i = 0; i < zGrid; i++) {
    int cap;
    inf >> cap;
    hCap->push_back(cap);
  }

  // LLX, LLY, tWidth, tHeight
  inf >> llx >> lly >> tWidth >> tHeight;  

  // Make a new instance
  rst = new RoutingInst(xGrid, yGrid, zGrid, *vCap, *hCap, llx, lly, tWidth, tHeight);

  // Parse nets
  inf >> dummy >> dummy >> numNets;
  for (int i = 0; i < numNets; i++) {
    string netName;
    int netId;
    int numPins;
    int netSize;		// Ignored
    inf >> netName >> netId >> numPins >> netSize;

    Net *n = new Net(netName, netId, numPins, llx, lly, tWidth, tHeight);
    // Parse pins
    for (int j = 0; j < numPins; j++) {
      point3d p;
      inf >> p.x >> p.y >> p.z;
      n->addPin(p);
    }
    rst->addNet(n);
  }

  // Blockages
  inf >> numBlockages;
  cout << numBlockages;

  for (int i = 0; i < numBlockages; i++) {
    point3d p1, p2;
    int cap;
    inf >> p1.x >> p1.y >> p1.z;
    inf >> p2.x >> p2.y >> p2.z;
    inf >> cap;
    rst->addBlockage(p1, p2, cap);
  }

  return rst;
}


int release(RoutingInst *rst){
  /*********** TO BE FILLED BY YOU **********/

  return 1;
}
  

