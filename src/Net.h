#ifndef NET_H
#define NET_H

#include <string>
#include <vector>
#include <stdio.h>

#include "util.h"


using std::string;
using std::vector;

/*
*  Net.h - class for a net 
*/

class Net {
 private:
  string name; 	        /* Name of the net */
  int id; 		/* ID of the net */
  int pinNum;     	/* Number of pins (or terminals) of the net */

  int llx, lly;
  int tWidth, tHeight;

  vector<point3d> pPins; 	/* An array of pins in the placement grid (3d) */
  vector<point3d> gPins;	/* An array of pins in the global routing grid (2d) */
  int routeNum;     	/* Number of (candidate) routes of the net. This may be equal to one (only one candidate route) in your implementation. */
  route r;	/* An array of candidate routes of the net. */

  int ofl;                      // Ofl of this Net's route - must be set by rst
  
 public:
  /*constructor*/
  Net (string name, int id, int pinNum, int llx, int lly, int tWidth, int tHeight);
  
  /*Public functions*/
  void addPin(point3d pin);
  void setRoute(route r);
  route getRoute();
  void setOfl(int ofl);
  int getOfl();

  string getName();
  void reorderPins(); 	

  /* Output */
  void printRoute(FILE *of);
  void printInput();

  /* Get global routing grid pins */
  vector<point3d> &getGPins();
};

#endif
