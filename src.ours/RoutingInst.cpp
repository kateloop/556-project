#include "RoutingInst.h"

RoutingInst::RoutingInst (int xGrid_in, int yGrid_in, int layer_in, int *vCap_in, int *hCap_in, int llx_in, int lly_in, int tWidth_in, int tHeight_in) {
	xGrid = xGrid_in;
	yGrid = yGrid_in;
	layer = layer_in;
	vCap = vCap_in;
	hCap = hCap_in;
	llx = llx_in;
	lly = lly_in;
	tWidth = tWidth_in;
	tHeight = tHeight_in;
	numNet = 0;
	edgeNum = 0;
	edgeNum2d = 0;
}

/*void add_net (net net_in) {
	nets[numNet] = net_in;
	numNet++;
}*/


