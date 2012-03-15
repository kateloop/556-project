// ECE556 - Copyright 2011 University of Wisconsin-Madison.  All Rights Reserved.
// $HeadURL:  $
// $Id: util.cpp 152 2011-01-19 02:05:45Z shojaei $


/*! \file util.cpp
    \brief source code for the ECE556 API.
    
    Details.
*/

  #include "ece556.h"


/********************************************************************************
 *   TODO : Re-implement these functions using our classes
 ********************************************************************************/



  
  // /*! \fn int get3DEdgeID(const point3d p1, const point3d p2, routingInst *rst)
  // 	    \brief Returns the edge id that connects p1 and p2
	
  // 	    \param input: first node of the edge 
  // 	    \param input: first node of the edge 
  // 	    \param input: pointer to rst (the routing instance)
  // 	    \returns edge id in 3D graph if successful, -1 otherswise(e.g. the points are not valid not valid) /
  // */
  // int get3DEdgeID(const point3d p1, const point3d p2, routingInst *rst){
  	
  // 	int edgeCount2D = (rst->yGrid-1)*rst->xGrid*(rst->layer/2) + (rst->xGrid-1)*rst->yGrid*(rst->layer/2);
  // 	int edgeCountLayer = 0;
  // 	int layerp = (p1.z <= p2.z) ? p1.z : p2.z;
	
  // 	edgeCountLayer = ((rst->xGrid-1)*rst->yGrid + rst->xGrid*(rst->yGrid-1))*(layerp/2);
		
	
  // 	// horizontal edge
  // 	if ((p1.y == p2.y) && (p1.z == p2.z))
  // 	{
  // 		if (p2.x - p1.x == 1)
  // 			return p1.y*(rst->xGrid-1) + p1.x + edgeCountLayer;
  // 		else if (p1.x - p2.x == 1)
  // 			return p1.y*(rst->xGrid-1) + p2.x + edgeCountLayer;
  // 		else
  // 		{
  // 			// error, do nothing	
  // 		}
  // 	}
  // 	else if ((p1.x == p2.x) && (p1.z == p2.z))
  // 	{
  // 		if (p2.y - p1.y == 1)
  // 			return rst->yGrid*(rst->xGrid-1) + p1.x*(rst->yGrid-1) + p1.y + edgeCountLayer;
  // 		else if (p1.y - p2.y == 1)
  // 			return rst->yGrid*(rst->xGrid-1) + p1.x*(rst->yGrid-1) + p2.y + edgeCountLayer;
  // 		else
  // 		{
  // 			// error, do nothing	
  // 		}
  // 	}
  // 	else if ((p1.x == p2.x) && (p1.y == p2.y))
  // 	{
  // 		if (p2.z - p1.z == 1)
  // 			return edgeCount2D + (p1.x*(rst->yGrid)+p1.y)*(rst->layer-1) + p1.z ;
  // 		else if (p1.z - p2.z == 1)
  // 			return edgeCount2D + (p2.x*(rst->yGrid)+p2.y)*(rst->layer-1) + p2.z ;		
  // 		else
  // 		{
  // 			// error, do nothing	
  // 		}
  // 	}
  // 	else
  // 	{
  // 		// error, do nothing
  // 	}

	
	
  // 	fprintf (stderr, "can't get edge1 :");
  // 	return -1;
  	
  // }
  
  // /*! \fn int getPoints3D(int edgeID, point3d &p1, point3d &p2, routingInst *rst)
  // 	    \brief set coordinates of p1 and p2 based on the edge id;
		
  // 		\param input: edge ID.
  // 	    \param input: first node of the edge
  // 	    \param input: second node of the edge
  // 	    \param input: pointer to rst (the routing instance)
  // 	    \returns 1 if successful
  // */
  // int getPoints3D(int edgeID, point3d &p1, point3d &p2, routingInst *rst){
  	
  // 	bool isVia;
  // 	int edgeCount2D = (rst->yGrid-1)*rst->xGrid*(rst->layer/2) + (rst->xGrid-1)*rst->yGrid*(rst->layer/2);
	
  // 	isVia = (edgeID < edgeCount2D) ? false : true;
		
  // 	if (isVia)
  // 	{
  // 		edgeID -= edgeCount2D;
		
  // 		p1.x = edgeID/(rst->xGrid*(rst->layer-1));
  // 		edgeID %= rst->xGrid*(rst->layer-1);
  // 		p1.y = edgeID/(rst->layer-1);
  // 		p1.z = edgeID%(rst->layer-1);
  // 		p2.x = p1.x;
  // 		p2.y = p1.y;
  // 		p2.z = p1.z;
		
  // 		p2.z +=1;
  // 	}
  // 	else
  // 	{
  // 		// get z (rst->layer)
  // 		p1.z = (edgeID / ((rst->yGrid-1)*rst->xGrid + rst->yGrid*(rst->xGrid-1))) * 2;		
  // 		p2.z = p1.z;		
		
  // 		edgeID = edgeID % ((rst->yGrid-1)*rst->xGrid + rst->yGrid*(rst->xGrid-1));

  // 		// horizontal edge
  // 		if (edgeID < rst->xGrid * (rst->yGrid - 1))
  // 		{
  // 			p1.x = edgeID % (rst->yGrid - 1);
  // 			p1.y = (edgeID - p1.x) / (rst->yGrid - 1);
  // 			p2.x = p1.x + 1;
  // 			p2.y = p1.y;
  // 		}
  // 		//vertical edge
  // 		else
  // 		{
  // 			int base = rst->xGrid * (rst->yGrid - 1);
		
  // 			p1.y = (edgeID - base) % (rst->xGrid - 1);
  // 			p1.x = (edgeID - base - p1.y) / (rst->xGrid - 1);
  // 			p2.x = p1.x;
  // 			p2.y = p1.y + 1;
			
  // 			p1.z += 1;
  // 			p2.z += 1;
  // 		}
  // 	}	
	
  // 	return 1;
  // }
  
  // /*! \fn int get2DEdgeID(const point3d p1, const point3d p2, routingInst *rst)
  // 	    \brief Returns the edge id that connects p1 and p2
	
  // 	    \param input: first node of the edge (will ignore p1.z)
  // 	    \param input: second (the other) node of the edge (will ignore p2.z)
  // 	    \param input: pointer to rst (the routing instance)
  // 	    \returns edge id in 2D graph if successful, -1 otherswise(e.g. the points are not valid not valid) 
  // */
  // int get2DEdgeID(const point3d p1, const point3d p2, routingInst *rst){
  	
  // 	// horizontal edge
  // 	if (p1.y == p2.y)
  // 	{
  // 		if (p2.x - p1.x == 1)
  // 			return p1.y*(rst->xGrid-1) + p1.x ;
  // 		else if (p1.x - p2.x == 1)
  // 			return p1.y*(rst->xGrid-1) + p2.x ;
  // 		else
  // 		{
  // 			// error, do nothing	
  // 		}
  // 	}
  // 	else if (p1.x == p2.x)
  // 	{
  // 		if (p2.y - p1.y == 1)
  // 			return rst->yGrid*(rst->xGrid-1) + p1.x*(rst->yGrid-1) + p1.y ;
  // 		else if (p1.y - p2.y == 1)
  // 			return rst->yGrid*(rst->xGrid-1) + p1.x*(rst->yGrid-1) + p2.y ;
  // 		else
  // 		{
  // 			// error, do nothing	
  // 		}
  // 	}
  // 	else
  // 	{
  // 		// error, do nothing
  // 	}
	
  // 	fprintf (stderr, "can't get edge1 :");
  // 	return -1;
  	
  // }
  
  // /*! \fn int getPoints2D(int edgeID, point3d &p1, point3d &p2, routingInst *rst)
  // 	    \brief set coordinates of p1 and p2 based on the edge id;
		
  // 		\param edge ID.
  // 	    \param first pin.
  // 	    \param second pin.
  // 	    \param rst the routing instance pointer.
  // 	    \returns 1 if successful
  // */
  // int getPoints2D(int edgeID, point3d &p1, point3d &p2, routingInst *rst){
  
  // 		p2.z = p1.z=0;		
		
  // 		// horizontal edge
  // 		if (edgeID < rst->xGrid * (rst->yGrid - 1))
  // 		{
  // 			p1.x = edgeID % (rst->yGrid - 1);
  // 			p1.y = (edgeID - p1.x) / (rst->yGrid - 1);
  // 			p2.x = p1.x + 1;
  // 			p2.y = p1.y;
  // 		}
  // 		//vertical edge
  // 		else
  // 		{
  // 			int base = rst->xGrid * (rst->yGrid - 1);
		
  // 			p1.y = (edgeID - base) % (rst->xGrid - 1);
  // 			p1.x = (edgeID - base - p1.y) / (rst->xGrid - 1);
  // 			p2.x = p1.x;
  // 			p2.y = p1.y + 1;
			
  // 			p1.z += 1;
  // 			p2.z += 1;
  // 		}
  // 		return 1;
  // }

