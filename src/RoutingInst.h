/*
 *  RoutingInst.h - Class for a Routing Instance
 */

#ifndef ROUTINGINST_H
#define ROUTINGINST_H

#include <vector>
#include <map>
#include <set>

#include "util.h"
#include "Net.h"

using std::vector;
using std::map;
using std::set;

typedef pair<edge, int> blockage;

class RoutingInst
{
 public:
  /****************************************
   *  Public access
   ****************************************/
  RoutingInst (int xGrid_in, int yGrid_in, int zGrid_in, vector<int> &vCap_in, vector<int> &hCap_in, int llx_in, int lly_in, int tWidth_in, int tHeight_in);

  /* Printing functions */
  void printRoute(char *outFile);
  void printGPins(vector<point3d> &gPins);

  /*Public functions*/
  void addNet (Net *n);
  void addBlockage(point3d p1, point3d p2, int cap);
  void printInput();
  void solveRouting();

  friend void *doRoutingTask(void *);
  

  /****************************************
   *  Problem size definition
   ****************************************/
  int xGrid; /*The x dimension of the global routing grid*/
  int yGrid; /*The y dimension of the global routing grid*/
  int zGrid; /*The z dimension of the global routing grid (ie number of layers)*/
	
  vector<int> &vCap; /*An array of the default vertical capacity of each layer (dimension = # layers)*/
  vector<int> &hCap; /*An array of the default horizontal capacity of each layer (dimension = # layers)*/
	
  int llx; /*lower left x of the offset starting point of the global routing grid*/
  int lly; /*lower left y of the offset starting point of the global routing grid*/
  int tWidth; /*tile width of one global bin*/
  int tHeight; /*tile height of one global bin*/

  int numNet;       /*Number of nets*/

  /****************************************
   *  Problem definition
   ****************************************/
  /* Nets */
  vector<Net> nets;		/* Nets to solve */
	
  /* Blockages */
  vector<blockage> blockages;	/* Blockages */
  map<edge, bool, edgeComp> isBlocked; /* Blockage lookup */

  /* Edge capacities */
  map<edge, int, edgeComp> edgeCap; /* 2D edge capacity utilization (Global routing grid) */
  map<edge, bool, edgeComp> edgeCapInitd; /* Edge capacity initialized */

  /****************************************
   *  Internal functions
   ****************************************/
  /* Safely set and get capacities */
  void setCap(edge e, int cap);
  int getCap(edge e);

  /* Is an edge vertical or horizontal */
  bool isVertical(edge e);
  bool isHorizontal(edge e);
  /* Get a suitable vertical or horizontal layer */
  int getVLayer();
  int getHLayer();

  /* Find a route for a net */
  route findRoute(Net&);

  /****************************************
   *  Routing Algorithms
   ****************************************/
  route bfsRoute(Net &n);

  /* Get neighboring points */
  set<point3d> getNeighborPoints(point3d p);

};

#endif // ROUTINGINST_H



/* Threaded routing task */
typedef struct {
  RoutingInst *rst;
  int modulo;
  int threads;
} routeTask;

void *doRoutingTask(void *);    
