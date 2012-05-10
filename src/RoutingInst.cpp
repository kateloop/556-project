#include "RoutingInst.h"

#include <stdio.h>
#include <time.h>
#include <queue>
#include <stack>

#include <pthread.h>

using namespace std;

RoutingInst::RoutingInst (int xGrid, int yGrid, int zGrid, vector<int> &vCap, vector<int> &hCap, int llx, int lly, int tWidth, int tHeight) :
  xGrid(xGrid), 
  yGrid(yGrid),
  zGrid(zGrid),
  vCap(vCap), 
  hCap(hCap),
  llx(llx),
  lly(lly),
  tWidth(tWidth),
  tHeight(tHeight)
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

  // Adjust capacities for this edge
  int rcap = getCap(e);
  if (isVertical(e)) {
    rcap -= vCap[e.first.z - 1]; // z index starts at 1..
    rcap += cap;
  } else { // horizontal
    rcap -= hCap[e.first.z - 1]; // z index starts at 1..
    rcap += cap;
  }
  setCap(e, rcap);
}


/********************************************************************************
 *  solveRouting - Prepares routing tasks to solve this routing instance
 ********************************************************************************/
#define MAXTHREADS 10
int NUMTHREADS = 1;

void RoutingInst::solveRouting()
{
  pthread_t threads[MAXTHREADS];
  int iret[MAXTHREADS];
  routeTask tasks[MAXTHREADS];

  printf("Routing %d nets\n", nets.size());
  for (int i = 0; i < NUMTHREADS; i++) {
    routeTask rt;
    tasks[i].rst = this;
    tasks[i].modulo = i;
    tasks[i].threads = NUMTHREADS;
    while (iret[i] = pthread_create(&threads[i], NULL, &doRoutingTask, (void *)&tasks[i])) {
      perror("Creating threads\n");
    }
  }

  for (int i = 0; i < NUMTHREADS; i++)
    pthread_join(threads[i], NULL);
}

// Threaded task

pthread_mutex_t netLock = PTHREAD_MUTEX_INITIALIZER;

void *doRoutingTask(void *task)
{
  routeTask *rt = (routeTask *)task;
  RoutingInst *rst = rt->rst;
  int modulo = rt->modulo;
  int threads = rt->threads;

  printf("Routing %d, %d\n", modulo, threads);
  fflush(stdout);
  
  for (int i = 0; i < rst->nets.size(); i++) {
    if (i % threads == modulo) {
      route r = rst->findRoute(rst->nets[i]);

      pthread_mutex_lock(&netLock);
      rst->nets[i].addRoute(r);
      pthread_mutex_unlock(&netLock);
    }

    // Print status update
    int res = 500;
    if (i % res == 0)
      printf("(%d/%d) routed\n", i, rst->nets.size());
  }
}

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

route RoutingInst::findRoute(Net &n)
{
  // Find a route on the 2d routing grid
  route r = route2d(n, &RoutingInst::bfs);
  
  // Find a suitable vertical and horizontal layer
  int vLayer, hLayer;
  vLayer = getVLayer();
  hLayer = getHLayer();
  
  // Insert necessary vias between edges 
  for (int i = 0; i < r.size(); i++) {
    // Grab the current edge
    edge &e = r[i];
    
    // Set layer for this type of edge
    if (isVertical(e)) {
      e.first.z = vLayer;
      e.second.z = vLayer;
    } else if (isHorizontal(e)) {
      e.first.z = hLayer;
      e.second.z = hLayer;      
    } else {
      printf("Non horizontal/vertical edge\n");
    }
    
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

/* get capacity of edge, initialize if needed */
int RoutingInst::getCap(edge e)
{
  // Initialize if necessary
  if (!edgeCapInitd[e]) {
    edgeCapInitd[e] = true;
    if (isVertical(e)) {
      for (int i = 0; i < vCap.size(); i++)
	edgeCap[e] += vCap[i];
    } else {
      for (int i = 0; i < hCap.size(); i++)
	edgeCap[e] += hCap[i];
    }
  }
  return edgeCap[e];
}

/* write over previous capacity */
void RoutingInst::setCap(edge e, int cap)
{
  edgeCapInitd[e] = true;
  edgeCap[e] = cap;
}

bool RoutingInst::isVertical(edge e)
{
  return e.first.y != e.second.y;
}

bool RoutingInst::isHorizontal(edge e)
{
  return e.first.x != e.second.x;
}

/********************************************************************************
 *  Routing Algorithms
 ********************************************************************************/
route RoutingInst::route2d(Net &n, route (RoutingInst::*routePins)(point3d, point3d))
{
  route r;
  vector<point3d> pins = n.getGPins();
  for (int i = 0; i < pins.size() - 1; i++) {
    // Route this pin pair
    point3d start = pins[i];
    point3d goal  = pins[i+1];
    route cur = (this->*routePins)(start, goal);

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
        else
          r.push_back(e);
      }
    }
  }
  return r;
}

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

route RoutingInst::bfs(point3d start, point3d goal)
{
  route r;
  priority_queue<point3d, vector<point3d>, L2Comp> open((L2Comp(goal)));
  //queue<point3d> open;
  map<point3d, bool> visited;
  map<point3d, point3d> prev;
  map<point3d, bool> started;
  
  // Start at start
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
        //printf("(%d,%d)\n", p.x, p.y);
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
        edge e = makeEdge(p, *it);
        if (!started[*it] &&
            !isBlocked[e] &&
            (*it).x >= 0  &&
            (*it).y >= 0) {
          prev[*it] = p;
          started[*it] = true;
          open.push(*it);
        }
      }
    }
  }
  return r;
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
  printf("Horizontal capacities:\n");
  for (int y = 0; y < yGrid; y++) {
    for (int x = 0; x < xGrid-1; x++) {
      edge e;
      e.first.x = x;
      e.second.x = x+1;
      e.first.y = y;
      e.second.y = y;
      printf("%d ", getCap(e));
    }
    printf("\n");
  }
  
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
}

int RoutingInst::getVLayer() {
  int vLayer = -1;
  for (int i = 0; i < vCap.size(); i++) {
    if (vCap[i]) {
      vLayer = i;
      break;
    }
  }
  return vLayer;
}

int RoutingInst::getHLayer() {
  int hLayer = -1;
  for (int i = 0; i < hCap.size(); i++) {
    if (hCap[i]) {
      hLayer = i;
      break;
    }
  }
  return hLayer;
}

void RoutingInst::printGPins(vector<point3d> &gPins) {
  for (int i = 0; i < gPins.size(); i++)
    printf("%d, %d, %d\n", gPins[i].x, gPins[i].y, gPins[i].z);
}

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
