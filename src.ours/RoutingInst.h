/*
 *  RoutingInst.h - Class for a Routing Instance
 */

#ifndef ROUTINGINST_H
#define ROUTINGINST_H

#include <vector>
#include <map>

#include "util.h"
#include "Net.h"

using std::vector;
using std::map;

typedef pair<edge, int> blockage;

class RoutingInst
{
 public:
	/*Constructor*/
	RoutingInst (int xGrid_in, int yGrid_in, int zGrid_in, vector<int> &vCap_in, vector<int> &hCap_in, int llx_in, int lly_in, int tWidth_in, int tHeight_in);
	
	/*Public functions*/
	void addNet (Net n);
	void addBlockage(point3d p1, point3d p2, int cap);
	void printInput();
	void solveRouting();

 private:
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
	vector<Net> nets; /* Nets */
	
	vector<blockage> blockages; /* Blocked edges */
	map<edge, bool, edgeComp> edgeCapInitd; /* Edge capacity initialized */
	map<edge, int, edgeComp> edgeCap; /* Edge capacity utilization (Global routing grid) */

	route findRoute(Net&);

};

#endif // ROUTINGINST_H
