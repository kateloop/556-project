/*
 *  RoutingInst.h - Class for a Routing Instance
 */

#ifndef ROUTINGINST_H
#define ROUTINGINST_H

/********************************************************************************
 *  Includes
 ********************************************************************************/
#include <vector>
#include <map>
#include <set>
#include <cmath>

#include "util.h"
#include "Net.h"

using std::vector;
using std::map;
using std::set;
using std::abs;

/********************************************************************************
 *  Local definitions
 ********************************************************************************/
typedef pair<edge, int> blockage;

// Manhattan distance comparison function for points
class L2Comp {
  point3d goal;
  int l2Dist(const point3d p)
  {
    return
      abs((double)(goal.x - p.x)) + 
      abs((double)(goal.y - p.y));
  }
public:
  L2Comp(point3d goal) : 
    goal(goal) {}
  
  bool operator()(const point3d p1, const point3d p2)
  {
    // Want shortest distance -> highest value
    // i.e. p1 value is less, if its distance is greater
    //return p1.z > p2.z;
    return l2Dist(p1) + 10 * p1.z > l2Dist(p2) + 10 * p2.z;
    //return l2Dist(p1) > l2Dist(p2);
  }
  private:
  L2Comp();
};

/********************************************************************************
 *  RoutingInst - a class to hold nets to be routed
 ********************************************************************************/
class RoutingInst
{
  /****************************************
   *  Public access
   ****************************************/
 public:
  // C'tor
  RoutingInst (int xGrid_in, int yGrid_in, int zGrid_in, vector<int> &vCap_in, vector<int> &hCap_in, int llx_in, int lly_in, int tWidth_in, int tHeight_in);

  // Printing
  void printRoute(char *outFile);
  void printGPins(vector<point3d> &gPins);

  // Routes
  void addNet (Net *n);
  void addBlockage(point3d p1, point3d p2, int cap);
  void printInput();
  void printCaps();
  void solveRouting();

  /****************************************
   *  Problem size definition
   ****************************************/
private:
  int xGrid;            /*The x dimension of the global routing grid*/
  int yGrid;            /*The y dimension of the global routing grid*/
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
  vector<Net> nets;             // Nets to solve
	
  /* Blockages */
  vector<blockage> blockages;            /* Blockages */
  map<edge, bool, edgeComp2d> isBlocked; /* Blockage lookup */

  /* Edge capacities */
  map<edge, int, edgeComp2d> edgeCap2d;       // 2 dimensional edge capacities
  map<edge, bool, edgeComp2d> edgeCapInitd2d; // 2 dimensional cap initialization

  map<edge, vector<int>, edgeComp2d> zCap; // Per-layer capacities
  map<edge, bool, edgeComp2d> zCapInitd;   // Per-layer initialized

  int totalOverflow;                      // Total overflow count
  int totalWireLength;                    // Total wirelength


  /****************************************
   *  Internal functions
   ****************************************/
  /* Safely set and get capacities */
  void setCap(edge e, int cap);
  int getCap(edge e);

  void setZCap(edge e, int layer, int cap);
  int getZCap(edge e, int layer);
  vector<int>& getZCap(edge e);

  // Add/remove routes to Nets
  void addRoute(route r, Net &n);
  void removeRoute(Net &n);

  int getTotalWireLength();
  int getTotalOverflow();

  int getRouteWireLength(route &r);
  int getRouteOverflow(route &r);

  vector<edge> getDecomposedEdges(route &r);
  vector<edge> getDecomposedEdge(edge &e);

  /****************************************
   *  3D routing
   ****************************************/
  // Find a 3-dimensional (complete) route
  route route3d(Net&, route r);

  /* Get a suitable vertical or horizontal layer */
  int getVLayer(edge e);
  int getHLayer(edge e);

  /****************************************
   *  2D routing
   ****************************************/
  // Find a 2-dimensional (global routing grid) route
  route route2d(Net &n, route (RoutingInst::*routePins)(point3d, point3d)=&RoutingInst::lshape);

  // Route 2 pins together
  route bfs(point3d start, point3d goal);
  route lshape(point3d start, point3d goal);

  /* Get neighboring points */
  set<point3d> getNeighborPoints(point3d p);

};

#endif // ROUTINGINST_H
