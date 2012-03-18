#ifndef ROUTINGINST_H
#define ROUTINGINST_H


/*
 *  RoutingInst.h - Class for a Routing Instance
 */

class RoutingInst
{
    public:
	int xGrid; /*The x dimension of the global routing grid*/
	int yGrid; /*The y dimension of the global routing grid*/
	int layer; /*The z dimension of the global routing grid (ie number of layers)*/

	int *vCap; /*An array of the default vertical capacity of each layer (dimension = # layers)*/
	int *hCap; /*An array of the default horizontal capacity of each layer (dimension = # layers)*/

	int llx; /*lower left x of the offset starting point of the global routing grid*/
	int lly; /*lower left y of the offset starting point of the global routing grid*/
	int tWidth; /*tile width of one global bin*/
	int tHeight; /*tile height of one global bin*/

	int numNet; /*Number of nets*/
//	net *nets; /*Array of nets*/

	int edgeNum; /*Number of edges in (3D grid)*/
	int *edgeCaps; /*Array of the actual edge capacities considering blockage (in the 3D grid so dimension = edgeNum)*/
	int *edgeUtils; /*Array of edge utilizations in the 3D grid graph (will be used during layer assignment in project part 2) */

	int edgeNum2d; /*Number of 2d edges in the projected 2D grid*/
	int *edgeCaps2d; /*Array of actual edge capacities in the projected 2d grid graph (so dimension = edgeNum2d), should be modified after reading blockage*/
	int *edgeUtils2d; /*Array of edge utilizations in the 2D grid graph (may be used during rip-up and reroute in project part 2*/

	/*Constructor*/
	RoutingInst (int xGrid_in, int yGrid_in, int layer_in, int *vCap_in, int *hCap_in, int llx_in, int lly_in, int tWidth_in, int tHeight_in);
	
	/*Public functions*/
//	void add_net (net net_in);
};

#endif // ROUTINGINST_H
