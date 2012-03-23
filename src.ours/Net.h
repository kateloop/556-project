#ifndef NET_H
#define NET_H

#include <string>
#include <vector>

/*
*  Net.h - class for a net 
*/

class Net {
 private:
  string name; 	        /* Name of the net */
  int id; 		/* ID of the net */
  int pinNum;     	/* Number of pins (or terminals) of the net */
  vector<point3d> *pins; 	/* An array of pins (or terminals) of the net. */
  int routeNum;     	/* Number of (candidate) routes of the net. This may be equal to one (only one candidate route) in your implementation. */
  vector<route> routes;	/* An array of candidate routes of the net. */
  
 public:
  /*constructor*/
  Net (string name_in, int id_in, int pinNum_in);
  
  /*Public functions*/
  void addPin(point3d pin);
};

#endif
