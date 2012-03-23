// ECE556 - Copyright 2011 University of Wisconsin-Madison.  All Rights Reserved.
// $HeadURL:  $
// $Id: ECE556.h 152 2011-02-18 02:19:45Z shojaei $


/*! \file ECE556.h
    \brief Header for the ECE556 API.
    
    Details.
*/

#ifndef ECE556_H
#define ECE556_H

// Standard lib includes
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// Local includes
#include "util.h"
#include "RoutingInst.h"

  /*! \fn int readBenchmark(const char *fileName, routingInst *rst)
	    \brief Read in the benchmark file and initialize the routing instance.
	    	   This function needs to populate all fields of the routingInst structure.
	    	   Use the following formula to translate the pin coordinates from the placement grid to the 3D global routing grid
	    	   xRouting = (xPlacement - rst->llx)/rst->tWidth where
	    	   yRouting = (yPlacement - rst->lly)/rst->tHeight
	    	   zRouting = zPlacement - 1  (zPlacement is 1 in all the benchmarks (so the cell pin are all located at metal layer M1))
	
	    \param fileName: Name of the benchmark input file
	    \param rst: pointer to the routing instance
	    \returns 1 if successful
  */


int readBenchmark(const char *fileName, RoutingInst *rst);
  /*! \fn int release(routingInst *rst)
    	\brief Release the memory for all the allocated data structures. 
	       Failure to release will likely cause memory problems after multiple runs of your program. 
               Need to recursively delete all memory allocations from bottom to top 
	       (starting from segments then routes then individual fields within a net struct, 
	       then nets, then the fields in a routing instance, and finally the routing instance)

    	\returns 1 if successful, 0 otherswise 
  */
 int release(RoutingInst *rst);

	
#endif // ECE556_H

