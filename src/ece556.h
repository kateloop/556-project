// ECE556 - Copyright 2011 University of Wisconsin-Madison.  All Rights Reserved.
// $HeadURL:  $
// $Id: ECE556.h 152 2011-02-18 02:19:45Z shojaei $


/*! \file ECE556.h
    \brief Header for the ECE556 API.
    
    Details.
*/

#ifndef ECE556_H
#define ECE556_H

#include <stdio.h>

 /**
  * A structure to represent a 3D Point. 
  */
 typedef struct
 {
   int x ; /* The x coordinate (>=0 in the routing grid)*/
   int y ; /* The y coordinate (>=0 in the routing grid)*/
   int z ; /* The z coordinate (>=0 in the routing grid)*/

 } point3d ;


  /**
  * A structure to represent a segment
  */
 typedef struct
 {
   point3d one ; 	/* The start point of a segment */
   point3d two ; 	/* The end point of a segment */
   
   int edgeNum ; 	/* Number of edges in the segment*/
   int *edges ;  	/* An array of edges representing the segment*/
   
 } segment ;
 
 
  /**
  * A structure to represent a route
  */
  typedef struct
  {
    int segNum ;  	/* Number of segments in a route*/
    segment *segments ;  /* An array of segments (note, a segment may be flat, L-shaped or any other shape, based on your preference */

  } route ;
 
 
  /**
  * A structure to represent nets
  */
  typedef struct
  {

   char *name ; 	/* Name of the net */
   int id ; 		/* ID of the net */
   int pinNum ; 	/* Number of pins (or terminals) of the net */
   point3d *pins ; 	/* An array of pins (or terminals) of the net. */
   int routeNum ; 	/* Number of (candidate) routes of the net. This may be equal to one (only one candidate route) in your implementation. */
   route *routes ;	/* An array of candidate routes of the net. */

  } net ;
  
  /**
  * A structure to represent the routing instance
  */
  typedef struct
  {
   int xGrid ;		/* The x dimension of the global routing grid */
   int yGrid ;		/* The y dimension of the global routing grid */
   int layer ;		/* The z dimension of the global routing grid (also known as number of layers) */
   
   int *vCap ;		/* An array of the default vertical capacity of each layer (dimension = #layers)*/
   int *hCap ;		/* An array of the default horizontal capacity of each layer (dimension = #layers)*/
   
   int llx ;		/* lower_left_x of the offset starting point of the global routing grid*/
   int lly ;		/* lower_left_y of the offset starting point of the global routing grid*/
   int tWidth ;		/* tile_width of one global bin*/
   int tHeight ;	/* tile_height of one global bin*/
   
   int numNet ;		/* Number of nets */
   net *nets ;		/* Array of nets */
   
   int edgeNum ; 	/* Number of edges in (3D grid) */
   int *edgeCaps; 	/* Array of the actual edge capacities considering blockage (in the 3D grid so dimension = edgeNum)*/
   int *edgeUtils;	/* Array of edge utilizations in the 3D grid graph (will be used during layer assignment in project part 2) */
   
   int edgeNum2d ; 	/* Number of 2d edges in the projected 2D grid */
   int *edgeCaps2d;     /* Array of actual edge capacities in the projected 2d grid graph (so dimension = edgeNum2d), should be modified after reading blockage */
   int *edgeUtils2d;    /* Array of edge utilizations in the 2D grid graph (may be used during rip-up and reroute in project part 2) */
   
  } routingInst ;
  

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
  int readBenchmark(const char *fileName, routingInst *rst);
  
  /*! \fn int projectTo2D(routingInst *rst)
      	\brief project the routing grid into a 2D routing grid 
	       need to compute the edge capacities in the projected 2D graph
               the utility functions get2DEdgeID, get3DEdgeID, getPoints2D, getPoints3D may be used
	       and can help to identify the indexes (and hence capacities) of the edges above each other in the 3D graph
	       (the syntax of these utility functions are given later in this file.)
	       (the utility functions are already implemented for your assistance inside the utility.cpp file.)

	\param rst the routing instance pointer.
    	\returns 1 if successful, 0 otherswise(e.g. the data structures are not populated) 
  */
  int projectTo2D(routingInst *rst);
  
  /*! \fn int solveRouting(routingInst *rst)
  	\brief This function creates a routing solution for the 2D routing problem. 
	       Here you need to work with the generated 2D global routing grid and 
	       assume all the nets should be routed on this 2D grid. A 3D solution will
	       be created in a separate 'layer assignment phase' function (not included
	       in the first part of the project).

	\param rst the routing instance pointer.
    	\returns 1 if successful, 0 otherswise(e.g. the data structures are not populated) 
  */
int solveRouting(routingInst *rst);
  
  /*! \fn int writeOutput(const char *outRouteFile, routingInst *rst)
    	\brief Write the routing solution obtained from solveRouting(). Refer to the project link for the output format
    		   For nets you may use the following formula to change the point coordinates:
	    	   xPlacement = xRouting*tWidth + llx
	    	   yPlacement = yRouting*tHeight + lly
	    	   zPlacement = zRouting+1	 
		   
		   Local nets (with terminals of all same coordinates in the global routing grid) must be ignored.
		   Map a 2D routing solution to a 3D routing solution by using only metal layers 1 and 2 
		   for the horizontal and vertical segments of each route fragment, respectively. 
		   Consecutive fragments of a route in two metal layers should be connected with a via.
		   (See the sampl output file in the project page to see how vias should be specified.)
		   Finally, make sure your generated output file passes the evaluation script to make sure
		   it is in the correct format and the nets have been correctly routed. The script also reports
		   the total wirelength and overflow of your routing solution.

    	\param outRouteFile: The name of the output file
    	\param rst: pointer to the routing instance
    	\returns 1 if successful, 0 otherswise(e.g. the output file is not valid) 
  */
  int writeOutput(const char *outRouteFile, routingInst *rst);
  
  /*! \fn int release(routingInst *rst)
    	\brief Release the memory for all the allocated data structures. 
	       Failure to release will likely cause memory problems after multiple runs of your program. 
               Need to recursively delete all memory allocations from bottom to top 
	       (starting from segments then routes then individual fields within a net struct, 
	       then nets, then the fields in a routing instance, and finally the routing instance)

    	\returns 1 if successful, 0 otherswise 
  */
 int release(routingInst *rst);


  /*! \fn int get2DEdgeID(const point3d p1, const point3d p2, routingInst *rst)
	    \brief Returns the edge id that connects p1 and p2
	
	    \param input: first node of the edge (will ignore p1.z)
	    \param input: second (the other) node of the edge (will ignore p2.z)
	    \param input: pointer to rst (the routing instance)
	    \returns edge id in 2D graph if successful, -1 otherswise(e.g. the points are not valid not valid) 
  */
  int get2DEdgeID(const point3d p1, const point3d p2, routingInst *rst);

  /*! \fn int get3DEdgeID(const point3d p1, const point3d p2, routingInst *rst)
	    \brief Returns the edge id that connects p1 and p2
	
	    \param input: first node of the edge 
	    \param input: first node of the edge 
	    \param input: pointer to rst (the routing instance)
	    \returns edge id in 3D graph if successful, -1 otherswise(e.g. the points are not valid not valid) /
  */
  int get3DEdgeID(const point3d p1, const point3d p2, routingInst *rst);

  /*! \fn int getPoints2D(int edgeID, point3d &p1, point3d &p2, routingInst *rst)
	    \brief Set coordinates of p1 and p2 based on the edge ID;
		
	    \param input: edge ID in 2D graph
	    \param output: first node of the edge (p1.z = 0)
	    \param output: second (the other) node of the edge (p2.z = 0)
	    \param input: pointer to rst (the routing instance)
	    \returns 1 if successful
  */
  int getPoints2D(int edgeID, point3d &p1, point3d &p2, routingInst *rst);
  
  /*! \fn int getPoints3D(int edgeID, point3d &p1, point3d &p2, routingInst *rst)
	    \brief set coordinates of p1 and p2 based on the edge id;
		
            \param input: edge ID.
	    \param input: first node of the edge
	    \param input: second node of the edge
	    \param input: pointer to rst (the routing instance)
	    \returns 1 if successful
  */
  int getPoints3D(int edgeID, point3d &p1, point3d &p2, routingInst *rst);
  
  

	
#endif // ECE556_H

