#include "RoutingInst.h"
#include <stdio.h>
#include <time.h>
#include <queue>
#include <stack>
#include <algorithm>


#include <pthread.h>

using namespace std;

int DEBUG_OFL = 0;

bool netCompByOverflow(Net n1, Net n2)
{
  return n1.getOfl() > n2.getOfl();
}

/********************************************************************************
 *  RoutingInst construction 
 ********************************************************************************/
RoutingInst::RoutingInst (int xGrid, int yGrid, int zGrid, vector<int> &vCap, vector<int> &hCap, int llx, int lly, int tWidth, int tHeight) :
  xGrid(xGrid), 
  yGrid(yGrid),
  zGrid(zGrid),
  vCap(vCap), 
  hCap(hCap),
  llx(llx),
  lly(lly),
  tWidth(tWidth),
  tHeight(tHeight),
  totalOverflow(0),
  totalWireLength(0)
{
}

void RoutingInst::addNet(Net *n)
{
  nets.push_back(*n);
}

void RoutingInst::addBlockage(point3d p1, point3d p2, int cap)
{
  blockage b;
  edge e;
  e.first = p1;
  e.second = p2;
  b.first = e;
  b.second = cap;
  blockages.push_back(b);
  isBlocked[e] = true;
  setCap(e, cap);
}



/********************************************************************************
 *  solveRouting - Solves this RoutingInst
 ********************************************************************************/
void RoutingInst::solveRouting()
{
  /****************************************
   * Initial Solution
   ****************************************/
  for (int i = 0; i < nets.size(); i++) {
    // Reorder this nets pins
    nets[i].reorderPins();

    // Find initial 2d solution
    route r2d = route2d(nets[i]);

    // Find 3d solution
    route r3d = route3d(nets[i], r2d);

    // Set this nets route
    nets[i].setRoute(r3d);
    // Adjust internal capacities
    addRoute(r3d);

    printCaps();
  }

  // All nets routed, give overflow
  for (int i = 0; i < nets.size(); i++) {
    route r = nets[i].getRoute();
    nets[i].setOfl(getRouteOverflow(r));
  }

  // Print stats before rip-up and re-route
  printf("=== Before Rip-up and Re-route ===\n");
  printf("Total wirelength: %d\n", getTotalWireLength());
  printf("Total overflow: %d\n", getTotalOverflow());
  printf("Debug overflow: %d\n", DEBUG_OFL);
  printf("==================================\n\n");

  /****************************************
   *  Rip-up and Re-route
   ****************************************/
  // Sort nets by ofl
  sort(nets.begin(), nets.end(), &netCompByOverflow);

  int TOF = 0;
  for (int i = 0; i < nets.size(); i++) {
    route original = nets[i].getRoute();
    int ofl = getRouteOverflow(original);
    if (ofl <= 0)
      continue;

    //printf("%d/%d :: Re-routing net with %d overflow... ", i, nets.size(), ofl);
        //    printf("\n");
        printCaps();

    // Remove old route
    nets[i].setRoute(route());
    removeRoute(nets[i].getRoute());

    // Find a new one
    route r2d = route2d(nets[i], &RoutingInst::bfs);
    route r3d = route3d(nets[i], r2d);
    nets[i].setRoute(r3d);
    nets[i].setOfl(getRouteOverflow(r3d));
    addRoute(r3d);

    int newOfl = nets[i].getOfl();
    //printf("found route with %d ofl.  %d  \n", newOfl, getTotalOverflow());
    fflush(stdout);
  }
  printf("TOF: %d\n", getTotalOverflow());
}


/********************************************************************************
 *  Output
 ********************************************************************************/
void RoutingInst::printRoute(char *outFile)
{
  FILE *of = fopen(outFile, "w");

  // Done - write to file
  printf("Writing solution to file...\n");
  fflush(stdout);
  for (int i = 0; i < nets.size(); i++) {
    nets[i].printRoute(of);
  }
}


/********************************************************************************
 *  3D Routing
 ********************************************************************************/
route RoutingInst::route3d(Net &n, route r)
{
  // Find a suitable vertical and horizontal layer
  int vLayer, hLayer;
  
  // Insert necessary vias between edges 
  for (int i = 0; i < r.size(); i++) {
    // Grab the current edge
    edge &e = r[i];
    
    // If the previous edge was on a different layer, add a via
    if (i > 0) {		// Not needed on first edge
      edge &prev = r[i-1];
      if (prev.second.z != e.first.z) { // Via needed
	// Connect
	edge via = makeEdge(prev.second, e.first);
	// Insert via, then try again
	r.insert(r.begin() + i, via);
      }
    }
  }

  // Insert vias to pins
  vector<point3d> gPins = n.getGPins();
  for (int i = 0; i < gPins.size(); i++) {
    // Create a via from the top, through to bottom
    point3d pinTop, pinBottom;
    pinTop.x = pinBottom.x = gPins[i].x;
    pinTop.y = pinBottom.y = gPins[i].y;
    pinTop.z = 0;
    pinBottom.z = zGrid-1;
    edge via = makeEdge(pinTop, pinBottom);
    // Insert via
    r.push_back(via);
  }
  return r;
}


// Get a free vertical layer
int RoutingInst::getVLayer(edge e) {
  int vLayer = -1;

  vector<int> &caps = getZCap(e);

  // Try to find a valid layer first
  for (int i = 0; i < caps.size(); i++)
    if (caps[i] > 0)
      return i;

  // Else, return a default one
  DEBUG_OFL++;
  for (int i = 0; i < vCap.size(); i++) {
    if (vCap[i]) {
      vLayer = i;
      break;
    }
  }
  return vLayer;
}

// Get a free horizontal layer
int RoutingInst::getHLayer(edge e) {
  int hLayer = -1;

  vector<int> &caps = getZCap(e);

  // Try to find a valid layer first
  for (int i = 0; i < caps.size(); i++) {
    if (caps[i] > 0)
      return i;
  }

  // Else, return a default one
  DEBUG_OFL++;
  for (int i = 0; i < hCap.size(); i++) {
    if (hCap[i]) {
      hLayer = i;
      break;
    }
  }
  return hLayer;
}


/********************************************************************************
 *  2D Routing Grid
 ********************************************************************************/
/* get capacity of edge, initialize if needed */
int RoutingInst::getCap(edge e)
{
  // Initialize if necessary
  if (!edgeCapInitd2d[e]) {
    edgeCapInitd2d[e] = true;
    if (isVertical(e)) {
      for (int i = 0; i < vCap.size(); i++)
      //      for (int i = 0; i < 1; i++)
	edgeCap2d[e] += vCap[i];
    } else {
      for (int i = 0; i < hCap.size(); i++)
      //      for (int i = 0; i < 1; i++)
	edgeCap2d[e] += hCap[i];
    }
  }
  return edgeCap2d[e];
}

/* write over previous capacity */
void RoutingInst::setCap(edge e, int cap)
{
  edgeCapInitd2d[e] = true;
  edgeCap2d[e] = cap;
}

int RoutingInst::getZCap(edge e, int layer)
{
  return zCap[e][layer];
}

void RoutingInst::setZCap(edge e, int layer, int cap)
{
  zCapInitd[e] = true;
  zCap[e][layer] = cap;
}

vector<int>& RoutingInst::getZCap(edge e)
{
  // Initialize if necessary
  if (!zCapInitd[e]) {
    zCapInitd[e] = true;
    if (isVertical(e))
      zCap[e] = vector<int> (vCap);
    else if (isHorizontal(e))
      zCap[e] = vector<int> (hCap);
    else
      zCap[e] = vector<int> (9999999);
  }
  return zCap[e];
}

// Adjust capacities for new route
void RoutingInst::addRoute(route r)
{
  vector<edge> edges = getDecomposedEdges(r);
  for (int i = 0; i < edges.size(); i++) {
    edge e = edges[i];
    int cap = getCap(e);
    setCap(e, cap-1);

    if (!isVia(e)) {
      int cap = getZCap(e, e.first.z);
      //      printf("Edge %s on layer %d has cap %d\n", edgeToString(e).c_str(), e.first.z, cap);
      setZCap(e, e.first.z, cap-1);
    }
  }
}

// Adjust capacities by removing a route
void RoutingInst::removeRoute(route r)
{
  vector<edge> edges = getDecomposedEdges(r);
  for (int i = 0; i < edges.size(); i++) {
    edge e = edges[i];
    int cap = getCap(e);
    setCap(e, cap+1);

    if (!isVia(e)) {
      int cap = getZCap(e, e.first.z);
      setZCap(e, e.first.z, cap+1);
    }
  }
}

// Returns the 3D wirelength for all nets' routes
int RoutingInst::getTotalWireLength()
{
  int wl = 0;
  for (int i = 0; i < nets.size(); i++) {
    route r = nets[i].getRoute();
    wl += getRouteWireLength(r);
  }
  return wl;
}

// Returns a route's 2D overflow
int RoutingInst::getRouteOverflow(route &r)
{
  int ofl = 0;
  vector<edge> edges = getDecomposedEdges(r);
  for (int i = 0; i < edges.size(); i++) {
    edge e = edges[i];
    if (isVia(e))
      continue;
    else if (getCap(e) >= 0)
      continue;
    else
      ofl++;
  }
  return ofl;
}

// Returns the sum of negative edge capacities that are not vias
int RoutingInst::getTotalOverflow()
{
  int ofl = 0;
  for (int i = 0; i < nets.size(); i++) {
    route r = nets[i].getRoute();
    ofl += getRouteOverflow(r);
  }
  return ofl;
}

// Returns wirelength for this route
int RoutingInst::getRouteWireLength(route &r)
{
  int wl = 0;
  vector<edge> edges = getDecomposedEdges(r);
  return edges.size();
}

// Returns edges of length 1 (breaks edges of length > 1 down) given route
vector<edge> RoutingInst::getDecomposedEdges(route &r)
{
  vector<edge> edges;
  for (int i = 0; i < r.size(); i++) {
    vector<edge> decomposed = getDecomposedEdge(r[i]);
    for (int j = 0; j < decomposed.size(); j++) {
      edges.push_back(decomposed[j]);
    }
  }
  return edges;
}

// Returns edges of length 1 given an edge
vector<edge> RoutingInst::getDecomposedEdge(edge &e)
{
  vector <edge> edges;
  if (isVertical(e)) {
    int starty = min(e.first.y, e.second.y);
    int endy   = max(e.first.y, e.second.y);
    for (int y = starty; y < endy; y++) {
      point3d pfirst, psecond;
      pfirst.x = e.first.x;
      pfirst.y = y;
      pfirst.z = e.first.z;
      psecond.x = e.first.x;
      psecond.y = y+1;
      psecond.z = e.first.z;
      if (starty == e.first.y)
        edges.push_back(edge(pfirst, psecond));
      else
        edges.insert(edges.begin(), edge(psecond, pfirst));
    }
  } else if (isHorizontal(e)) {
    int startx = min(e.first.x, e.second.x);
    int endx   = max(e.first.x, e.second.x);
    for (int x = startx; x < endx; x++) {
      point3d pfirst, psecond;
      pfirst.x = x;
      pfirst.y = e.first.y;
      pfirst.z = e.first.z;
      psecond.x = x+1;
      psecond.y = e.first.y;
      psecond.z = e.first.z;
      if (startx == e.first.x)
        edges.push_back(edge(pfirst, psecond));
      else
        edges.insert(edges.begin(), edge(psecond, pfirst));

    }
  } else {                      // Via
    for (int i = 0; i < abs((double)e.first.z - e.second.z); i++)
      edges.push_back(e);
  }
  return edges;
}

/********************************************************************************
 *  2D Routing Algorithms
 ********************************************************************************/

// Get a route for this net in the 2D routing grid using the pin router
route RoutingInst::route2d(Net &n, route (RoutingInst::*routePins)(point3d, point3d))
{
  route r;
  vector<point3d> pins = n.getGPins();
  for (int i = 0; i < pins.size() - 1; i++) {
    // Route this pin pair
    point3d start = pins[i];
    point3d goal  = pins[i+1];
    route cur = (this->*routePins)(start, goal);

    vector<edge> edges = getDecomposedEdges(cur);
    for (int j = 0; j < edges.size(); j++)
      r.push_back(edges[j]);

    /*

      MERGING NET CODE - bad for z layer assignment

    // Append to Net's route
    for (int j = 0; j < cur.size(); j++) {
      edge e = cur[j];
      if (j == 0 || j == cur.size()-1)
        r.push_back(e);
      else {
        edge &prev = r[r.size()-1];
        if ((isVertical(e) && isVertical(prev)) ||
            isHorizontal(e) && isHorizontal(prev))
          prev.second = e.second; // pass through
        else {
          r.push_back(e);
        }
      }
    }
    */
  }

  // Z layer assignment
  for (int i = 0; i < r.size(); i++) {
    edge &e = r[i];
    if (isVertical(e)) {
      int vLayer = getVLayer(e);
      e.first.z = vLayer;
      e.second.z = vLayer;
    } else if (isHorizontal(e)) {
      int hLayer = getHLayer(e);
      int cap = getZCap(e, hLayer);
      e.first.z = hLayer;
      e.second.z = hLayer;          
    } else if (isVia(e)) {
    }

    //    if (e.first.z > 1)
    //      printf("Edge %s on layer %d\n", edgeToString(e).c_str(), getZCap(e, e.first.z));
  }
  return r;
}

// Route two pins using l-shapes
route RoutingInst::lshape(point3d start, point3d goal)
{
  // Connect horizontal, then vertical
  route r;
  point3d corner;

  // Already there
  if (start == goal) {
    // Nothing to do
  }

  // Same x or same y, no bend needed
  else if (start.x == goal.x ||
           start.y == goal.y) {
    r.push_back(makeEdge(start, goal));
  }

  // Choose a corner
  else {
    corner.x = start.x;
    corner.y = goal.y;
    corner.z = start.z;
    
    // And connect
    r.push_back(makeEdge(start, corner));
    r.push_back(makeEdge(corner, goal));
  }
  return r;
}

// Route two pins using bfs
route RoutingInst::bfs(point3d start, point3d goal)
{
  route r;
  priority_queue<point3d, vector<point3d>, L2Comp> open((L2Comp(goal)));
  map<point3d, bool> visited;
  map<point3d, point3d> prev;
  map<point3d, bool> started;
  
  // Start at start
  start.z = 0;                  // OVERFLOW
  open.push(start);
  prev[start] = start;
  started[start] = true;
  
  while (!open.empty()) {
    // Visited?
    if (visited[open.top()]) {
      open.pop();
      continue;
    }

    // Mark visited
    point3d p = open.top(); open.pop();
    visited[p] = true;
    
    // Goal test
    if (p == goal) {
      stack<edge> s;
      
      // Retrace and reverse
      while (p != start) {
        edge e;
        e.first = prev[p];
        e.second = p;
        s.push(e);
        p = prev[p];
      }
      while (!s.empty()) {
        r.push_back(s.top());
        s.pop();
      }
      break;		// Start next pin
    }
    
    // Search
    else {
      // Add new edges to search
      set<point3d> nextPoints = getNeighborPoints(p);
      
      for (set<point3d>::iterator it = nextPoints.begin(); it != nextPoints.end(); it++) {
        point3d next = *it;
        edge e = makeEdge(p, next);
        if (!started[next] &&
            !isBlocked[e] &&
            next.x >= 0  &&
            next.y >= 0) {
          int cap = getCap(e);
          int ofl = 0;
          if (cap <= 0)
            ofl++;
          next.z = p.z + ofl;
          prev[next] = p;
          started[next] = true;
          open.push(next);
          //          printf("Trying %s\t\t::%d\n", edgeToString(e).c_str(), next.z);
        }
      }
    }
  }
  return r;
}


// Find neighboring points - used to generate next-search locations in routing
set<point3d> RoutingInst::getNeighborPoints(point3d p)
{
  set<point3d> pts;
  point3d up, left, down, right;
  up.x = down.x = p.x;          // Up/Down    same X
  left.y = right.y = p.y;       // Left/Right same Y
  up.y = p.y + 1;               // Up
  down.y = p.y - 1;             // Down
  right.x = p.x + 1;            // Right
  left.x = p.x - 1;             // Left

  pts.insert(up);
  pts.insert(left);
  pts.insert(down);
  pts.insert(right);

  return pts;
}









/********************************************************************************
 *  Debug printing
 ********************************************************************************/

void RoutingInst::printInput()
{
  printf("grid %d %d %d\n", xGrid, yGrid, zGrid);
  printf("vertical capacity ");
  for (int i = 0; i < vCap.size(); i++)
    printf("%d ", vCap[i]);
  printf("\nhorizontal capacity ");
  printf("\nPlacement grid:\n");
  for (int i = 0; i < hCap.size(); i++)
    printf("%d ", hCap[i]);
  printf("\nGlobal routing grid:\n");
  printf("\n%d %d %d %d\n", llx, lly, tWidth, tHeight);
  for (int i = 0; i < nets.size(); i++)
    nets[i].printInput();
  printf("Blockages:\n");
  for (vector<blockage>::iterator it = blockages.begin(); it != blockages.end(); it++) {
    blockage b = *it;
    edge e = b.first;
    int cap = b.second;
    point3d p1 = e.first;
    point3d p2 = e.second;
    printf("Blockage: (%d, %d, %d) -> (%d, %d, %d) : %d\n", p1.x, p1.y, p1.z,
	   p2.x, p2.y, p2.z, cap);
  }

  printCaps();
}

void RoutingInst::printCaps()
{
  printf("Horizontal capacities:\n");
  for (int y = 0; y < yGrid; y++) {
    for (int x = 0; x < xGrid-1; x++) {
      if (x % 10 != 0)
        continue;
      edge e;
      e.first.x = x;
      e.second.x = x+1;
      e.first.y = y;
      e.second.y = y;
      //      printf("%d ", edgeCapInitd2d[e] < 0 ? 0 : 1);
      printf("%d ", getCap(e) < 35 && !isBlocked[e] ? 1 : 0);
    }
    printf("\n");
  }
  printf("\n\n");

  /*  
  printf("Vertical capacities:\n");
  for (int y = 0; y < yGrid-1; y++) {
    for (int x = 0; x < xGrid; x++) {
      edge e;
      e.first.x = x;
      e.second.x = x;
      e.first.y = y;
      e.second.y = y+1;
      printf("%d ", getCap(e));
    }
    printf("\n");
  }
  */
}

void RoutingInst::printGPins(vector<point3d> &gPins) {
  for (int i = 0; i < gPins.size(); i++)
    printf("%d, %d, %d\n", gPins[i].x, gPins[i].y, gPins[i].z);
}
