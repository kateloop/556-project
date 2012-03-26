#include "RoutingInst.h"

#include <stdio.h>

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

void RoutingInst::addNet(Net n)
{
  nets.push_back(n);
}

void RoutingInst::addBlockage(point3d p1, point3d p2, int cap)
{
  edge e;
  e.first = p1;
  e.second = p2;
  edgeUtilization[e] = cap;
}

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
  for (map<edge, int>::iterator it = edgeUtilization.begin(); it != edgeUtilization.end(); it++) {
    edge e = (*it).first;
    int cap = (*it).second;
    point3d p1 = e.first;
    point3d p2 = e.second;
    printf("Blockage: (%d, %d, %d) -> (%d, %d, %d) : %d\n", p1.x, p1.y, p1.z,
	   p2.x, p2.y, p2.z, cap);
  }
}

void RoutingInst::solveRouting()
{
  printf("Routing\n");
  for (vector<Net>::iterator it = nets.begin(); it != nets.end(); it++) {
    (*it).addRoute(findRoute(*it));
  }
}

route RoutingInst::findRoute(Net n)
{

}
