#ifndef NET_H
#define NET_H

#include "ece556.h"

/*
*  net.h - class for a net 
*/

class net {
  public:
   	char *name; 	/* Name of the net */
   	int id; 		/* ID of the net */
   	int pinNum; 	/* Number of pins (or terminals) of the net */
   	point3d *pins; 	/* An array of pins (or terminals) of the net. */
   	int routeNum; 	/* Number of (candidate) routes of the net. This may be equal to one (only one candidate route) in your implementation. */
   	route *routes;	/* An array of candidate routes of the net. */

   	/*constructor*/
   	net (char *name_in, int id_in, int pinNum_in, point3d *pins_in);
	
	/*Public functions*/
	void add_route (route route);
};

#endif
