// ECE556 - Copyright 2011 University of Wisconsin-Madison.  All Rights Reserved.
// $HeadURL:  $
// $Id: main.cpp 152 2011-02-18 02:19:45Z shojaei $


/*! \file main.cpp
  \brief source code for the ECE556 API.
    
  Details.
*/

#include "ece556.h"
#include <stdlib.h>

extern int NUMTHREADS;

int main(int argc, char **argv)
{
  if (argc != 3) {
    printf("Usage : ./ROUTE.exe <input_benchmark_name> <output_file_name>\n");
    return 1;
  }

  int status;
  char *inputFileName = argv[1];
  char *outputFileName = argv[2];
  RoutingInst *rst;

  /// read benchmark
  rst = readBenchmark(inputFileName);

  // solve routing
  rst->solveRouting();

  // print result to file
  rst->printRoute(outputFileName);

  // release memory
  delete rst;

  printf("\nDONE!\n");	
  return 0;

  
  /**
   *  Simple test of hash functionality
   *
  point3d p1, p2, p3, p4;
  p1.x = 1;
  p1.y = 5;
  p1.z = 10;

  p2.x = 10;
  p2.y = 6;
  p2.z = 11;

  p3.x = p2.x;
  p3.y = p2.y;
  p3.z = p2.z-1;

  edge e1 = edge(p1, p2);       // An edge
  edge e2 = edge(p2, p1);       // Reflection of edge
  edge e3 = edge(p3, p1);       // Reflection of edge with different z
  edge e4 = edge(p1, p3);       // Reflection of e3

  map<edge, int, edgeComp2d> m2;
  map<edge, int, edgeComp3d> m3;

  m2[e1] = 123;
  m3[e1] = 123;
  m3[e4] = 432;

  printf("2d :: %d %d\n", m2[e1], m2[e2]);
  printf("3d :: %d %d\n", m3[e1], m3[e3]);
  **/
  

}
