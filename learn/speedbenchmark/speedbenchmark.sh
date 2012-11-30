#!/bin/bash

make clean
make && echo "Done compiling... now doing the timing. Come back in an hour or so."
git log -n 1 --format='%ct,"%s","%H",' | tr -d '\n' >> speedbenchmark.csv || exit 1
./a.out >> speedbenchmark.csv
make clean
