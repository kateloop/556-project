#include "RoutingInst.h"

#include <stdio.h>
#include <time.h>
#include <queue>

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
}

void RoutingInst::solveRouting()
{
  /* Make blockage adjustments */
  for (int i = 0; i < blockages.size(); i++) {
    // Pull out blockage fields
    blockage b = blockages[i];
    edge e = b.first;
    int bcap = b.second;
    // Get routed capcacity
    int rcap = getCap(e);
    if (isVertical(e)) {
      rcap -= vCap[e.first.z - 1]; // z index starts at 1..
      rcap += bcap;
    } else { // horizontal
      rcap -= hCap[e.first.z - 1]; // z index starts at 1..
      rcap += bcap;
    }
    setCap(e, rcap);
    //printf("Setting capacity of edge to %d\n", rcap);
  }

  printf("Routing %d nets\n", nets.size());
  int ct = 0;
  const int res = 500;
  time_t start, last, end;
  time(&start);
  time(&last);
  for (int i = 0; i < nets.size(); i++) {
    nets[i].addRoute(findRoute(nets[i]));

    /* Status */
    if (!(++ct % res)) {
      time(&end);
      int elapsed = difftime(end, start);
      int diff = difftime(end, last);
      if (diff > 0)
	printf("%ds:  %d% (%d/%d) nets routed\t%d rps  (%d min remaining)\n", elapsed, ct*100/nets.size(), ct, nets.size(), res/diff, (nets.size() - ct) / (res/diff) / 60);
      time(&last);
    }
  }
}

void RoutingInst::printRoute(char *outFile)
{
  FILE *of = fopen(outFile, "w");

  // Done - write to file
  for (int i = 0; i < nets.size(); i++) {
    nets[i].printRoute(of);
  }
}

route RoutingInst::findRoute(Net &n)
{
  route r = bfsRoute(n);

  // Insert pins on actual z layer
  vector<point3d> gPins = n.getGPins();
  int pin = gPins.size()-1; // Pin we are working on

  for (int i = 0; i < gPins.size(); i++)
    printf("%d %d\n", gPins[i].x, gPins[i].y);

  // Find vertical and horizontal layers
  int vLayer, hLayer;
  for (int i = 0; i < vCap.size(); i++) {
    if (vCap[i]) {
      vLayer = i;
      break;
    }
  }
  for (int i = 0; i < hCap.size(); i++) {
    if (hCap[i]) {
      hLayer = i;
      break;
    }
  }

  // Add vias, move edges to correct layer, insert pins
  for (int i = 0; i < r.size(); i++) {
    edge &e = r[i];

    // Set layer
    if (isVertical(e)) {
      e.first.z = vLayer;
      e.second.z = vLayer;
    } else {
      e.first.z = hLayer;
      e.second.z = hLayer;      
    }

    // DEBUG
    printf("========================================\n\n");
    printf("Solving edge %s\n", edgeToString(e).c_str());

    // Add vias to pins if needed
    if (e.first == gPins[pin]) {
      
      printf("Edge.first %s is a pin\n", edgeToString(e).c_str());

      if (e.first.z != gPins[pin].z) {
	// Connect
	edge via = makeEdge(gPins[pin], e.first);

	// Insert
	r.insert(r.begin() + i, via);
      }
      i++;
      pin--;
    } else if (e.second == gPins[pin]) {

      printf("Edge.second %s is a pin\n", edgeToString(e).c_str());

      if (e.second.z != gPins[pin].z) {
        // Connect
	edge via = makeEdge(e.second, gPins[pin]);

	// Insert
	r.insert(r.begin() + i, via);
      }
      i++;
      pin--;
    }

    // Add vias to neighboring edges
    if (i > 0) {		// Not needed on first edge
      edge prev = r[i-1];
      if (prev.second.z != e.first.z) { // Via needed
	edge via;
	// Connect
	via.first.x = prev.second.x;
	via.first.y = prev.second.y;
	via.first.z = prev.second.z;
	via.second.x = e.first.x;
	via.second.y = e.first.y;
	via.second.z = e.first.z;
	// Insert
	r.insert(r.begin() + i, via);
      }
    }
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
  return !isVertical(e);
}

/********************************************************************************
 *  Routing Algorithms
 ********************************************************************************/
route RoutingInst::bfsRoute(Net &n)
{
  route r;
  vector<point3d> pins = n.getGPins();
  for (int i = 0; i < pins.size() - 1; i++) {
    point3d start = pins[i];
    point3d goal  = pins[i+1];

    queue<point3d> open;
    map<point3d, bool> visited;
    map<point3d, point3d> prev;
    map<point3d, bool> started;

    // Start at start
    open.push(start);
    prev[start] = start;
    started[start] = true;

    while (!open.empty()) {
      // Visited?
      if (visited[open.front()]) {
	open.pop();
	continue;
      }

      // Mark visited
      point3d p = open.front(); open.pop();
      visited[p] = true;

      // Goal test
      if (p == goal) {
	while (p != start) {
	  //printf("(%d,%d)\n", p.x, p.y);
	  edge e;
	  e.first = p;
	  e.second = prev[p];
	  r.push_back(e);
	  p = prev[p];
	}
	break;		// Start next pin
      }

      // Search
      else {
	// Create new search points
	point3d up, left, down, right;
	up.x = down.x = p.x;
	left.y = right.y = p.y;
	up.y = p.y + 1;
	down.y = p.y - 1;
	right.x = p.x + 1;
	left.x = p.x - 1;
	// Add to search queue
	edge e;
	e.first = p;
	e.second = up;
	if (!started[up] && !isBlocked[e] && up.x >= 0 && up.y >= 0) {
	  prev[up] = p;
	  started[up] = true;
	  open.push(up);
	}
	e.second = down;
	if (!started[down] && !isBlocked[e] && down.x >= 0 && down.y >= 0) {
	  prev[down] = p;
	  started[down] = true;
	  open.push(down);
	}
	e.second = left;
	if (!started[left] && !isBlocked[e] && left.x >= 0 && left.y >= 0) {
	  prev[left] = p;
	  started[left] = true;
	  open.push(left);
	}
	e.second = right;
	if (!started[right] && !isBlocked[e] && right.x >= 0 && right.y >= 0) {
	  prev[right] = p;
	  started[right] = true;
	  open.push(right);
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
