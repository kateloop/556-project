#ifndef UTIL_H
#define UTIL_H

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
 * A structure to represent an edge
 */
typedef struct
{
  point3d one;
  point3d two;
} edge;


/*
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


/* Placement routing grid coordinates to global routing grid */
point3d ptog(point3d p, int llx, int lly, int tWidth, int tHeight);

#endif