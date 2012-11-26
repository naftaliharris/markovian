#!/bin/bash

make clean
make && git log -n 1 --format='%ct,"%s","%H",' | tr -d '\n' >> speedbenchmark.csv || exit 1
./a.out >> speedbenchmark.csv
make clean
