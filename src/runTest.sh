#!/bin/bash

function fail() {
    echo Usage: runTest.sh path/to/router/input path/to/eval/input
    exit 0
}

# Check args
if [ $# -ne 2 ]; then
    echo Usage: runTest.sh path/to/router/input path/to/eval/input
    exit 0
fi

# Check files
if [ ! -f $1 ]; then
    echo $0: Cannot open $1
    fail
elif [ ! -f $2 ]; then
    echo $0: Cannot open $2
    fail
fi

# Make sure up to date
echo $0: Updating ROUTE.exe...
echo
make

# Run it
echo $0: Routing...
echo
time ./ROUTE.exe $1 $1.out 20 20

# Check the output
if [ -f $1.out ]; then
    echo $0: Running testbench...
    echo
    time ./eval2008/eval2008.pl $2 $1.out
fi
