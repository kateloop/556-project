#!/bin/bash

if [ $# -ne 1 ]; then
    echo Usage: threadBench.sh [numThreads]
    exit 0
fi

/usr/bin/time -f "Time: %E" ./ROUTE.exe infiles/adaptec1.10000.gr infiles/adaptec1.10000.gr.out $1 | grep Time
