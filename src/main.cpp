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
 	if(argc != 3 && 
           argc != 4){
 		printf("Usage : ./ROUTE.exe <input_benchmark_name> <output_file_name> [numThreads]\n");
 		return 1;
 	}

 	int status;
	char *inputFileName = argv[1];
 	char *outputFileName = argv[2];

        if (argc == 4) {
          NUMTHREADS = atoi(argv[3]);
          printf("Running with %d threads\n", NUMTHREADS);
        }


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
}
